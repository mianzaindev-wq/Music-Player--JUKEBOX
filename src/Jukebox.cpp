#include "../header/Jukebox.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdlib>

#ifdef _WIN32
#define UNICODE
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

Jukebox::Jukebox() : currentIndex(0) {
    loadDefaultFolders();
}

Jukebox::~Jukebox() {
    clearPlaylist();
}

void Jukebox::addMusicFolder(const std::string& folderPath) {
    if (fs::exists(folderPath) && fs::is_directory(folderPath)) {
        musicFolders.push_back(folderPath);
    }
}

void Jukebox::loadDefaultFolders() {
    std::string basePath = "music";
    if (fs::exists(basePath) && fs::is_directory(basePath)) {
        for (const auto& entry : fs::directory_iterator(basePath)) {
            if (entry.is_directory()) {
                musicFolders.push_back(entry.path().string());
            }
        }
    }
}

const std::vector<std::string>& Jukebox::getMusicFolders() const {
    return musicFolders;
}

void Jukebox::loadSongsFromDirectory(const std::string& directoryPath) {
    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                std::string extension = entry.path().extension().string();
                if (extension == ".mp3" || extension == ".wav" || extension == ".flac" ||
                    extension == ".ogg" || extension == ".m4a" || extension == ".aac") {
                    Song song;
                    song.name = entry.path().stem().string();
                    song.path = entry.path().string();
                    song.size = fs::file_size(entry.path());
                    playlist.push_back(song);
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error loading songs: " << e.what() << std::endl;
    }
}

void Jukebox::loadSongsFromFolder(size_t folderIndex) {
    if (folderIndex >= musicFolders.size()) return;
    clearPlaylist();
    loadSongsFromDirectory(musicFolders[folderIndex]);
    currentIndex = 0;
}

void Jukebox::clearPlaylist() {
    playlist.clear();
    currentIndex = 0;
}

void Jukebox::playSong(size_t songIndex) {
    if (songIndex >= playlist.size()) return;
    currentIndex = songIndex;

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time);

    std::ostringstream timeStream;
    timeStream << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    std::string timestamp = timeStream.str();

#ifdef _WIN32
    mciSendString(L"close mySong", NULL, 0, NULL);

    std::string mciOpenStr = "open \"" + playlist[currentIndex].path + "\" type mpegvideo alias mySong";
    std::wstring mciOpen(mciOpenStr.begin(), mciOpenStr.end());

    MCIERROR err = mciSendString(mciOpen.c_str(), NULL, 0, NULL);
    if (err) {
        std::cerr << "Error opening file (MCI Error Code: " << err << ")" << std::endl;
    } else {
        mciSendString(L"play mySong", NULL, 0, NULL);
    }

#elif __APPLE__
    std::string command = "afplay \"" + playlist[currentIndex].path + "\" &";
    std::system(command.c_str());
#else
    std::string command = "mpg123 \"" + playlist[currentIndex].path + "\" &";
    std::system(command.c_str());
#endif

    std::ofstream historyFile("../history.txt", std::ios::app);
    if (!historyFile.is_open()) {
        std::cerr << "Could not write to history.txt\n";
    } else {
        historyFile << "[" << timestamp << "] " << playlist[currentIndex].name << std::endl;
        historyFile.close();
    }
}

void Jukebox::nextSong() {
    if (!playlist.empty()) {
        currentIndex = (currentIndex + 1) % playlist.size();
    }
}

void Jukebox::previousSong() {
    if (!playlist.empty()) {
        currentIndex = (currentIndex == 0) ? playlist.size() - 1 : currentIndex - 1;
    }
}

void Jukebox::showHistory() const {
    std::ifstream historyFile("../history.txt");
    if (!historyFile.is_open()) {
        std::cout << "No history found.\n";
        return;
    }

    std::cout << "\n--- RECENTLY PLAYED ---\n";
    std::string line;
    int count = 1;
    while (std::getline(historyFile, line)) {
        std::cout << count++ << ". " << line << std::endl;
    }
    historyFile.close();
}

std::string Jukebox::getCurrentSongName() const {
    if (!playlist.empty()) {
        return playlist[currentIndex].name;
    }
    return "No Song Selected";
}

std::string Jukebox::getCurrentSongPath() const {
    if (!playlist.empty()) {
        return playlist[currentIndex].path;
    }
    return "";
}

std::string Jukebox::getCurrentSongSize() const {
    if (!playlist.empty()) {
        double sizeInMB = static_cast<double>(playlist[currentIndex].size) / (1024 * 1024);
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2) << sizeInMB << " MB";
        return stream.str();
    }
    return "0 MB";
}

std::vector<std::string> Jukebox::getPlaylistStrings() const {
    std::vector<std::string> list;
    if (playlist.empty()) {
        list.push_back("No songs in playlist.");
        return list;
    }

    for (size_t i = 0; i < playlist.size(); ++i) {
        std::string item = std::to_string(i + 1) + ". " + playlist[i].name;
        if (i == currentIndex) {
            item += " <-- [Current]";
        }
        list.push_back(item);
    }

    return list;
}

bool Jukebox::hasSongs() const {
    return !playlist.empty();
}
