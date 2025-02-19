# SDL2 MP3 Player

A simple console-based MP3 player that uses SDL2 for audio playback. This application allows you to load and play WAV audio files from a directory, navigate through a playlist, and control playback with simple keyboard commands.

## Features

- Load and play WAV audio files from a `songs` directory
- Navigate through playlist (next/previous song)
- Playback controls (play, pause, stop)
- Multi-threaded audio playback for responsive UI
- Support for immediate song switching

## Requirements

- C compiler (GCC or Clang)
- SDL2 library
- POSIX threads support

## Installation

1. Make sure SDL2 is installed on your system:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libsdl2-dev
   
   # macOS with Homebrew
   brew install sdl2
   ```

2. Clone the repository:
   ```bash
   git clone git@github.com:Joshua-Coded/Summative-Project.git
   cd Summative-Project/Question1
   ```

3. Compile the program:
   ```bash
   make
   ```

## Usage

1. Create a `songs` directory and add WAV files to it:
   ```bash
   mkdir -p songs
   cp /path/to/your/wav/files/*.wav songs/
   ```

2. Run the program:
   ```bash
   ./mp3_player
   ```

3. Control playback using these commands:
   - `n`: Play next song
   - `p`: Play previous song
   - `x`: Pause/Resume current song
   - `s`: Stop playback
   - `q`: Quit the program

## Project Structure

```
mp3-player/
├── include/            # Header files
│   └── mp3_player.h
├── src/                # Source files
│   └── mp3_player.c
├── main.c              # Main program
├── songs/              # Directory for WAV files
├── Makefile            # Build configuration
└── README.md           # This file
```

## Function Explanations

### Data Structure

```c
typedef struct SongNode {
    char songName[256];
    struct SongNode* next;
    struct SongNode* prev;
} SongNode;
```

This doubly linked list structure allows for efficient navigation through the playlist in both directions.

### Core Functions

1. **`initializePlaylist(SongNode** head)`**
   - **Purpose**: Creates an empty playlist
   - **Parameters**: Pointer to playlist head pointer
   - **Returns**: None
   - **Time Complexity**: O(1)

2. **`addSong(SongNode** head, const char* songName)`**
   - **Purpose**: Adds a new song to the end of the playlist
   - **Parameters**: Pointer to playlist head, song filename
   - **Returns**: None
   - **Time Complexity**: O(n) where n is the number of songs in the playlist

3. **`displayPlaylist(const SongNode* head)`**
   - **Purpose**: Displays all songs in the playlist
   - **Parameters**: Playlist head pointer
   - **Returns**: None
   - **Time Complexity**: O(n) where n is the number of songs in the playlist

4. **`playSong(const SongNode* current)`**
   - **Purpose**: Plays a song in a separate thread
   - **Parameters**: Pointer to the song node to play
   - **Returns**: None
   - **Time Complexity**: O(1) for starting playback

5. **`stopPlayback()`**
   - **Purpose**: Stops current song playback
   - **Parameters**: None
   - **Returns**: None
   - **Time Complexity**: O(1)

6. **`navigatePlaylist(SongNode** current)`**
   - **Purpose**: Handles user input to navigate through playlist
   - **Parameters**: Pointer to current song pointer
   - **Returns**: None
   - **Time Complexity**: O(1) per command

7. **`playAudioThread(void* arg)`**
   - **Purpose**: Thread function that handles actual audio playback
   - **Parameters**: Pointer to the current song node
   - **Returns**: NULL
   - **Time Complexity**: O(t) where t is the duration of the song

## Algorithm Analysis

### Playlist Management

#### Adding Songs (addSong)
- **Best Case**: O(1) when adding the first song (empty playlist)
- **Worst Case**: O(n) when adding to a playlist with n songs (traverses to the end)
- **Space Complexity**: O(1) additional space per song

#### Displaying Playlist (displayPlaylist)
- **Best Case**: O(1) for empty playlist
- **Worst Case**: O(n) where n is the number of songs in the playlist
- **Space Complexity**: O(1) additional space

### Playback Control

#### Song Navigation (navigatePlaylist)
- **Best Case**: O(1) for all operations (next, previous, pause, etc.)
- **Worst Case**: O(1) for all operations
- **Space Complexity**: O(1)

#### Audio Playback (playSong/playAudioThread)
- **Time Complexity**: O(1) to start playback in a new thread
- **Space Complexity**: O(s) where s is the size of the audio buffer

### Memory Management

The program uses a linked list to store the playlist, which requires O(n) memory for n songs. Each node in the list contains the song's filename and pointers to the next and previous songs.

When the program exits, it properly frees all allocated memory to prevent leaks.

## Performance Considerations

1. **Thread Management**: The program uses separate threads for audio playback to ensure the UI remains responsive.

2. **Large Playlists**: For very large playlists (thousands of songs), adding songs might become slower due to O(n) traversal. A potential optimization would be to maintain a tail pointer.

3. **File Loading**: Loading large audio files might be slow on systems with limited memory. The program loads the entire file into memory for playback.

