#include "../header/Jukebox.h"
#include <iostream>
#include <string>
#include <limits>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

const int BOX_WIDTH = 60;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printChars(char c, int count) {
    for (int i = 0; i < count; i++) {
        std::cout << c;
    }
}

void printLine(char left, char mid, char right) {
    std::cout << left;
    printChars(mid, BOX_WIDTH - 2);
    std::cout << right << "\n";
}

void printRow(const char* text, bool centered = false) {
    int len = 0;
    while (text[len] != '\0') len++;
    
    std::cout << "| ";
    
    if (centered) {
        int padding = (BOX_WIDTH - 4 - len) / 2;
        printChars(' ', padding);
        std::cout << text;
        printChars(' ', BOX_WIDTH - 4 - len - padding);
    } else {
        std::cout << text;
        printChars(' ', BOX_WIDTH - 4 - len);
    }
    
    std::cout << " |\n";
}

void printEmptyRow() {
    std::cout << "|";
    printChars(' ', BOX_WIDTH - 2);
    std::cout << "|\n";
}

void printHeader() {
    printLine('+', '=', '+');
    printRow("CLASSIC JUKEBOX", true);
    printLine('+', '=', '+');
}

void printMainMenu() {
    printEmptyRow();
    printRow("MAIN MENU", true);
    printEmptyRow();
    printLine('+', '-', '+');
    printRow("1. Play from existing folders");
    printRow("2. Add new music folder");
    printRow("3. View playback history");
    printRow("4. Exit");
    printEmptyRow();
    printLine('+', '=', '+');
    std::cout << "Choose an option (1-4): ";
}

void printFolderMenu(const std::vector<std::string>& folders) {
    printEmptyRow();
    printRow("SELECT MUSIC FOLDER", true);
    printEmptyRow();
    printLine('+', '-', '+');
    
    for (size_t i = 0; i < folders.size(); ++i) {
        std::string folderName = folders[i].substr(folders[i].find_last_of("/\\") + 1);
        printRow((std::to_string(i + 1) + ". " + folderName).c_str());
    }
    printRow((std::to_string(folders.size() + 1) + ". Back to Main Menu").c_str());
    printEmptyRow();
    printLine('+', '=', '+');
    std::cout << "Choose a folder (1-" << folders.size() + 1 << "): ";
}

void printSongMenu(const std::vector<std::string>& songs) {
    printEmptyRow();
    printRow("SELECT SONG TO PLAY", true);
    printEmptyRow();
    printLine('+', '-', '+');
    
    for (size_t i = 0; i < songs.size(); ++i) {
        printRow(songs[i].c_str());
    }
    printRow((std::to_string(songs.size() + 1) + ". Back to Folders").c_str());
    printEmptyRow();
    printLine('+', '=', '+');
    std::cout << "Choose a song (1-" << songs.size() + 1 << "): ";
}

void printPlayerMenu() {
    printEmptyRow();
    printRow("NOW PLAYING", true);
    printEmptyRow();
    printLine('+', '-', '+');
    printEmptyRow();
    printRow("Commands:");
    printRow("[P]lay/Pause  [N]ext  [B]ack  [L]ist  [H]istory  [Q]uit");
    printEmptyRow();
    printLine('+', '=', '+');
    std::cout << "Command > ";
}

void printCurrentSong(const Jukebox& jukebox) {
    printEmptyRow();
    printRow(("  > " + jukebox.getCurrentSongName()).c_str());
    printEmptyRow();
    printRow(("  Size: " + jukebox.getCurrentSongSize()).c_str());
    printEmptyRow();
    printRow(("  Path: " + jukebox.getCurrentSongPath()).c_str());
    printEmptyRow();
}

int getIntInput(int min, int max) {
    int choice;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail() || choice < min || choice > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}

char getCharInput() {
    char ch;
    std::cin >> ch;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return tolower(ch);
}

int main() {
    Jukebox jukebox;
    bool running = true;

    while (running) {
        clearScreen();
        printHeader();
        printMainMenu();
        
        int choice = getIntInput(1, 4);
        
        switch (choice) {
            case 1: { // Play from existing folders
                const auto& folders = jukebox.getMusicFolders();
                if (folders.empty()) {
                    clearScreen();
                    printHeader();
                    printEmptyRow();
                    printRow("No music folders found!", true);
                    printRow("Add folders first using option 2.", true);
                    printEmptyRow();
                    printLine('+', '=', '+');
                    std::cout << "Press Enter to continue...";
                    std::cin.get();
                    break;
                }
                
                bool inFolderMenu = true;
                while (inFolderMenu) {
                    clearScreen();
                    printHeader();
                    printFolderMenu(folders);
                    
                    int folderChoice = getIntInput(1, folders.size() + 1);
                    
                    if (folderChoice == folders.size() + 1) {
                        inFolderMenu = false;
                    } else {
                        jukebox.loadSongsFromFolder(folderChoice - 1);
                        
                        if (!jukebox.hasSongs()) {
                            clearScreen();
                            printHeader();
                            printEmptyRow();
                            printRow("No songs found in this folder!", true);
                            printEmptyRow();
                            printLine('+', '=', '+');
                            std::cout << "Press Enter to continue...";
                            std::cin.get();
                            continue;
                        }
                        
                        bool inSongMenu = true;
                        while (inSongMenu) {
                            clearScreen();
                            printHeader();
                            printSongMenu(jukebox.getPlaylistStrings());
                            
                            int songChoice = getIntInput(1, jukebox.getPlaylistStrings().size() + 1);
                            
                            if (songChoice == jukebox.getPlaylistStrings().size() + 1) {
                                inSongMenu = false;
                            } else {
                                jukebox.playSong(songChoice - 1);
                                
                                bool inPlayer = true;
                                while (inPlayer) {
                                    clearScreen();
                                    printHeader();
                                    printCurrentSong(jukebox);
                                    printPlayerMenu();
                                    
                                    char command = getCharInput();
                                    
                                    switch (command) {
                                        case 'p':
                                            jukebox.playSong(jukebox.getPlaylistStrings().size() - 1); // Play current
                                            break;
                                        case 'n':
                                            jukebox.nextSong();
                                            break;
                                        case 'b':
                                            jukebox.previousSong();
                                            break;
                                        case 'l':
                                            inPlayer = false;
                                            break;
                                        case 'h':
                                            clearScreen();
                                            printHeader();
                                            printEmptyRow();
                                            printRow("PLAYBACK HISTORY", true);
                                            printEmptyRow();
                                            printLine('+', '=', '+');
                                            std::cout << "\n";
                                            jukebox.showHistory();
                                            std::cout << "\nPress Enter to return...";
                                            std::cin.get();
                                            break;
                                        case 'q':
                                            inPlayer = false;
                                            inSongMenu = false;
                                            inFolderMenu = false;
                                            running = false;
                                            break;
                                        default:
                                            break;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            }
            case 2: { // Add new music folder
                clearScreen();
                printHeader();
                printEmptyRow();
                printRow("ADD NEW MUSIC FOLDER", true);
                printEmptyRow();
                printLine('+', '=', '+');
                std::cout << "Enter the path to the music folder: ";
                
                std::string newPath;
                std::getline(std::cin, newPath);
                
                if (!newPath.empty()) {
                    jukebox.addMusicFolder(newPath);
                    printEmptyRow();
                    printRow("Folder added successfully!", true);
                } else {
                    printEmptyRow();
                    printRow("No path entered.", true);
                }
                printEmptyRow();
                printLine('+', '=', '+');
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;
            }
            case 3: { // View playback history
                clearScreen();
                printHeader();
                printEmptyRow();
                printRow("PLAYBACK HISTORY", true);
                printEmptyRow();
                printLine('+', '=', '+');
                std::cout << "\n";
                jukebox.showHistory();
                std::cout << "\nPress Enter to return...";
                std::cin.get();
                break;
            }
            case 4: { // Exit
                running = false;
                break;
            }
        }
    }

    clearScreen();
    printHeader();
    printEmptyRow();
    printRow("Thank you for using Classic Jukebox!", true);
    printEmptyRow();
    printLine('+', '=', '+');
    std::cout << "\n";
    
    return 0;
}
