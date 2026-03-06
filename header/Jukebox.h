#ifndef JUKEBOX_H
#define JUKEBOX_H

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class Jukebox {
private:
    struct Song {
        std::string name;
        std::string path;
        uintmax_t size;
    };

    std::vector<Song> playlist;
    size_t currentIndex;
    std::vector<std::string> musicFolders;

    void loadSongsFromDirectory(const std::string& directoryPath);
    void clearPlaylist();

public:
    Jukebox();
    ~Jukebox();

    void addMusicFolder(const std::string& folderPath);
    void loadDefaultFolders();
    const std::vector<std::string>& getMusicFolders() const;
    void loadSongsFromFolder(size_t folderIndex);
    void playSong(size_t songIndex);
    void nextSong();
    void previousSong();
    void showHistory() const;
    std::string getCurrentSongName() const;
    std::string getCurrentSongPath() const;
    std::string getCurrentSongSize() const;
    std::vector<std::string> getPlaylistStrings() const;
    bool hasSongs() const;
};

#endif // JUKEBOX_H