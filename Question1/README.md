# MP3 Player Project

## Overview

This project implements an MP3 player using a doubly linked list in C. The tool allows users to navigate through a playlist of songs, play, pause, and stop audio. It supports seamless navigation between songs using `n` (next) and `p` (previous).

---

## Features

- **Playlist Management**: Songs are stored in a doubly linked list.
- **Navigation**: Users can move forward (`n`) or backward (`p`) through the playlist.
- **Playback Control**: Supports play, pause, and stop functionalities.
- **Boundary Handling**: Gracefully handles edge cases like reaching the start or end of the playlist.
- **Real-Time Audio Playback**: Integrates SDL library for actual audio playback.

---

## Data Structures

The core data structure used in this project is a **doubly linked list**. Each node in the list represents a song and contains:
- `songName`: Name of the song.
- `next`: Pointer to the next song.
- `prev`: Pointer to the previous song.

```c
typedef struct SongNode {
    char songName[256];
    struct SongNode* next;
    struct SongNode* prev;
} SongNode;
```

---

## Algorithm Analysis

### Adding Songs
- **Description**: Adds a new song to the end of the playlist.
- **Process**: Iterates through the list to find the last node and appends the new song.
- **Time Complexity**: O(n), where `n` is the number of songs in the playlist.

### Navigating Songs
- **Description**: Moves the current pointer to the next or previous node based on user input.
- **Process**: Updates the current pointer to the `next` or `prev` node.
- **Time Complexity**: O(1) for each navigation step.

### Displaying Playlist
- **Description**: Prints all song names in the playlist.
- **Process**: Traverses the entire list and prints each song name.
- **Time Complexity**: O(n), where `n` is the number of songs.

### Playback Control
- **Description**: Simulates playback by printing the current song name or playing it using SDL.
- **Process**: For simulation, prints the current song name. For real-time playback, uses SDL to load and play the song.
- **Time Complexity**: O(1).

---

## Best and Worst Case Analysis

### Adding a Song
- **Best Case**: O(1) when the playlist is empty, as no traversal is needed.
- **Worst Case**: O(n) when the playlist has many songs, requiring traversal to the end.

### Navigating Songs
- **Best Case**: O(1) when moving to the next or previous song.
- **Worst Case**: O(1) since navigation involves only pointer updates.

### Displaying Playlist
- **Best Case**: O(n) as all nodes must be traversed.
- **Worst Case**: O(n) as all nodes must be traversed.

### Playback Control
- **Best Case**: O(1) when the current song exists.
- **Worst Case**: O(1) since playback involves only loading and playing the song.

---

## Function Explanations

### `initializePlaylist(SongNode** head)`
- **Purpose**: Initializes the playlist as an empty doubly linked list.
- **Process**: Sets the head pointer to `NULL`.

```c
void initializePlaylist(SongNode** head) {
    *head = NULL;
}
```

### `addSong(SongNode** head, const char* songName)`
- **Purpose**: Adds a new song to the playlist.
- **Process**:
  - Allocates memory for a new node.
  - Copies the song name into the node.
  - Links the new node to the end of the list.
- **Edge Cases**: Handles memory allocation failures gracefully.

```c
void addSong(SongNode** head, const char* songName) {
    SongNode* newSong = malloc(sizeof(SongNode));
    if (!newSong) {
        printf("Memory allocation failed!\n");
        return;
    }
    strcpy(newSong->songName, songName);
    newSong->next = NULL;
    newSong->prev = NULL;

    if (*head == NULL) {
        *head = newSong;
    } else {
        SongNode* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newSong;
        newSong->prev = temp;
    }
}
```

### `displayPlaylist(const SongNode* head)`
- **Purpose**: Displays all songs in the playlist.
- **Process**: Traverses the list and prints each song name.
- **Edge Cases**: Handles an empty playlist by printing a message.

```c
void displayPlaylist(const SongNode* head) {
    if (head == NULL) {
        printf("Playlist is empty.\n");
        return;
    }

    printf("Current Playlist:\n");
    const SongNode* temp = head;
    while (temp != NULL) {
        printf("- %s\n", temp->songName);
        temp = temp->next;
    }
}
```

### `playSong(const SongNode* current)`
- **Purpose**: Simulates or plays the current song.
- **Process**:
  - For simulation, prints the current song name.
  - For real-time playback, uses SDL to load and play the song.
- **Edge Cases**: Handles cases where the current song is `NULL`.

```c
void playSong(const SongNode* current) {
    if (current == NULL) {
        printf("No song to play.\n");
        return;
    }
    printf("Now playing: %s\n", current->songName);

    // Real-time playback using SDL (if enabled)
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return;
    }

    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8* wavBuffer;

    if (SDL_LoadWAV(current->songName, &wavSpec, &wavBuffer, &wavLength) == NULL) {
        printf("Failed to load WAV file: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);

    while (SDL_GetQueuedAudioSize(deviceId) > 0) {
        SDL_Delay(100); // Wait for audio to finish
    }

    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();
}
```

### `navigatePlaylist(SongNode** current)`
- **Purpose**: Allows the user to navigate through the playlist.
- **Process**:
  - Accepts user input (`n` for next, `p` for previous, `q` to quit).
  - Updates the current pointer based on the input.
  - Handles boundary conditions (start/end of the playlist).
- **Edge Cases**: Prevents invalid navigation beyond the playlist boundaries.

```c
void navigatePlaylist(SongNode** current) {
    char command[10];
    while (1) {
        printf("\nEnter command (n: next, p: previous, q: quit): ");
        scanf("%s", command);

        if (strcmp(command, "n") == 0) {
            if ((*current)->next != NULL) {
                *current = (*current)->next;
                playSong(*current);
            } else {
                printf("End of playlist reached.\n");
            }
        } else if (strcmp(command, "p") == 0) {
            if ((*current)->prev != NULL) {
                *current = (*current)->prev;
                playSong(*current);
            } else {
                printf("Beginning of playlist reached.\n");
            }
        } else if (strcmp(command, "q") == 0) {
            printf("Exiting MP3 player.\n");
            break;
        } else {
            printf("Invalid command. Please try again.\n");
        }
    }
}
```

---

## How to Compile and Run

1. **Prerequisites**:
   - Install GCC (GNU Compiler Collection).
   - Install SDL2 for real-time audio playback:
     ```bash
     sudo apt-get install libsdl2-dev
     ```

2. **Compilation**:
   - Navigate to the project directory in the terminal.
   - Run the following command to compile the program:
     ```bash
     make
     ```

3. **Execution**:
   - Run the compiled program using:
     ```bash
     ./mp3_player
     ```
   - Follow the on-screen instructions to interact with the MP3 player.

4. **Testing**:
   - To test the program with sample songs, run:
     ```bash
     make test
     ```

5. **Cleanup**:
   - Remove object files and executables using:
     ```bash
     make clean
     ```

---

## Limitations

1. **Audio Playback**:
   - The current implementation assumes all songs are in `.wav` format due to SDL's default support. Support for other formats (e.g., `.mp3`) requires additional libraries.

2. **Error Handling**:
   - Limited error handling for invalid inputs or corrupted song files.

3. **Dynamic Memory Management**:
   - Assumes sufficient memory for all operations. If memory allocation fails, the program may crash.

---

## Future Enhancements

1. **Support for Multiple Audio Formats**:
   - Integrate libraries like FFmpeg to support `.mp3`, `.flac`, and other formats.

2. **Advanced Navigation**:
   - Add features like shuffle, repeat, and jump to a specific song.

3. **Graphical User Interface (GUI)**:
   - Develop a GUI using frameworks like GTK or Qt for better user interaction.

4. **Improved Error Handling**:
   - Handle edge cases like empty playlists, missing files, or invalid user input more gracefully.

---

## References

- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms*. MIT Press.
- SDL2 Documentation: https://wiki.libsdl.org/
- GNU Make Documentation: https://www.gnu.org/software/make/manual/

