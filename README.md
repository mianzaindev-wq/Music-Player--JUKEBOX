# Classic Jukebox

A simple, console-based music player written in C++ that organizes your music collection by genre folders.

## Features

- **Organized Music Library**: Automatically scans predefined genre folders (rock, kpop, jazz, etc.) in the `music/` directory.
- **Menu-Driven Interface**: Easy-to-use text-based menus for navigation.
- **Playback Controls**: Play, pause, next, previous, and playlist navigation.
- **Playback History**: Logs all played songs with timestamps.
- **Extensible**: Add new music folders dynamically.
- **Cross-Platform**: Supports Windows (with MCI), macOS, and Linux.

## Supported Formats

- MP3 (.mp3)
- WAV (.wav)
- FLAC (.flac)
- OGG (.ogg)
- M4A (.m4a)
- AAC (.aac)

## Project Structure

```
JukeBox Prototype/
├── header/
│   └── Jukebox.h          # Class declarations
├── src/
│   ├── main.cpp          # Main application
│   └── Jukebox.cpp       # Class implementations
├── music/                # Music folders (create subfolders for genres)
│   ├── rock/
│   ├── kpop/
│   └── jazz/
├── LICENSE               # MIT License
├── .gitignore           # Git ignore rules
├── README.md            # This file
└── main.exe             # Compiled executable (Windows)
```

## Prerequisites

- **Windows**: MSYS2 with MinGW-w64 (for g++ and winmm library)
- **macOS**: Xcode Command Line Tools (for clang++)
- **Linux**: GCC (for g++)

## Building

1. Ensure you have a C++ compiler installed.
2. Navigate to the project root directory.
3. Run the build command:

   ```bash
   g++ -fdiagnostics-color=always -g src/main.cpp src/Jukebox.cpp -lwinmm -o main.exe
   ```

   - On Windows: Include `-lwinmm` for multimedia support.
   - On macOS/Linux: Remove `-lwinmm`.

## Usage

1. Place your music files in subfolders under `music/` (e.g., `music/rock/song.mp3`).
2. Run the executable: `./main.exe` (or `main.exe` on Windows).
3. Use the menus to:
   - Select a music folder
   - Choose a song to play
   - Control playback with P (play/pause), N (next), B (previous), etc.
   - View playback history with H
   - Add new folders with option 2

## Controls

- **P**: Play/Pause current song
- **N**: Next song
- **B**: Previous song
- **L**: Show playlist
- **H**: Show playback history
- **Q**: Quit

## Adding Music

1. Create subfolders in `music/` for different genres (e.g., `music/pop/`, `music/classical/`).
2. Place your audio files in these folders.
3. The player will automatically detect them on next run.

## Playback History

Played songs are logged in `history.txt` with timestamps. This file is ignored by git to keep your listening history private.

## Contributing

1. Fork the repository.
2. Create a feature branch.
3. Make your changes.
4. Submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Disclaimer

This software is provided as-is for educational and personal use. Ensure you have legal rights to play the music files you use with this player.

## Future Plans:

- Will be working on UI and better Navigation in near Future.
