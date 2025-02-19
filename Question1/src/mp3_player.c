#include "mp3_player.h"
#include <stdbool.h>
#include <pthread.h>

// Global variables for playback control
volatile bool isPlaying = false;
volatile bool isPaused = false;
volatile bool shouldStop = false;
SDL_AudioDeviceID currentDeviceId = 0;
pthread_t playbackThread;

// Thread function for playing audio
void* playAudioThread(void* arg) {
    const SongNode* current = (const SongNode*)arg;
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8* wavBuffer;

    if (SDL_LoadWAV(current->songName, &wavSpec, &wavBuffer, &wavLength) == NULL) {
        printf("Failed to load WAV file: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    currentDeviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    SDL_QueueAudio(currentDeviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(currentDeviceId, 0);  // Start playing immediately

    // Wait until playback finishes or is interrupted
    while (!shouldStop && SDL_GetQueuedAudioSize(currentDeviceId) > 0) {
        if (isPaused) {
            SDL_PauseAudioDevice(currentDeviceId, 1);  // Pause
        } else {
            SDL_PauseAudioDevice(currentDeviceId, 0);  // Resume
        }
        SDL_Delay(100);
    }

    // Clean up resources
    SDL_CloseAudioDevice(currentDeviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();
    
    if (!shouldStop) {
        // Playback finished naturally
        isPlaying = false;
    }
    
    return NULL;
}

// Initialize an empty playlist
void initializePlaylist(SongNode** head) {
    *head = NULL;
}

// Add a song to the playlist
void addSong(SongNode** head, const char* songName) {
    SongNode* newNode = (SongNode*)malloc(sizeof(SongNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    
    strncpy(newNode->songName, songName, 255);
    newNode->songName[255] = '\0';
    newNode->next = NULL;
    
    if (*head == NULL) {
        // First song in the playlist
        newNode->prev = NULL;
        *head = newNode;
        return;
    }
    
    // Add to the end of the playlist
    SongNode* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    current->next = newNode;
    newNode->prev = current;
}

// Display all songs in the playlist
void displayPlaylist(const SongNode* head) {
    if (head == NULL) {
        printf("Playlist is empty.\n");
        return;
    }
    
    printf("\n===== Playlist =====\n");
    int songNumber = 1;
    const SongNode* current = head;
    
    while (current != NULL) {
        printf("%d. %s\n", songNumber++, current->songName);
        current = current->next;
    }
    printf("====================\n");
}

// Stop the current playback
void stopPlayback() {
    if (isPlaying) {
        shouldStop = true;
        
        // Stop the device immediately
        if (currentDeviceId > 0) {
            SDL_ClearQueuedAudio(currentDeviceId);
            SDL_PauseAudioDevice(currentDeviceId, 1);
        }
        
        // Wait for the thread to finish
        pthread_join(playbackThread, NULL);
        
        // Reset flags
        shouldStop = false;
        isPlaying = false;
        isPaused = false;
        currentDeviceId = 0;
    }
}

// Function to play a song using SDL2 in a separate thread
void playSong(const SongNode* current) {
    if (current == NULL) {
        printf("No song to play.\n");
        return;
    }

    // Stop any currently playing song
    stopPlayback();
    
    printf("Now playing: %s\n", current->songName);
    
    // Reset flags
    isPlaying = true;
    isPaused = false;
    shouldStop = false;
    
    // Create thread for audio playback
    if (pthread_create(&playbackThread, NULL, playAudioThread, (void*)current) != 0) {
        printf("Failed to create playback thread.\n");
        isPlaying = false;
        return;
    }
    
    // Make thread detached so resources are freed automatically
    pthread_detach(playbackThread);
}

// Navigate through the playlist
void navigatePlaylist(SongNode** current) {
    char command[10];
    printf("\nControls: n: next, p: previous, s: stop, q: quit, x: pause/resume\n");
    
    while (1) {
        printf("\nEnter command: ");
        scanf("%s", command);

        if (strcmp(command, "n") == 0) { // Next song
            if ((*current)->next != NULL) {
                *current = (*current)->next;
                playSong(*current);
            } else {
                printf("End of playlist reached.\n");
            }
        } else if (strcmp(command, "p") == 0) { // Previous song
            if ((*current)->prev != NULL) {
                *current = (*current)->prev;
                playSong(*current);
            } else {
                printf("Beginning of playlist reached.\n");
            }
        } else if (strcmp(command, "s") == 0) { // Stop playback
            if (isPlaying) {
                printf("Stopping playback...\n");
                stopPlayback();
            } else {
                printf("Playback already stopped.\n");
            }
        } else if (strcmp(command, "x") == 0) { // Pause/Resume
            if (isPlaying) {
                isPaused = !isPaused; // Toggle pause state
                if (isPaused) {
                    printf("Playback paused.\n");
                } else {
                    printf("Playback resumed.\n");
                }
            } else {
                printf("No song is currently playing.\n");
            }
        } else if (strcmp(command, "q") == 0) { // Quit the program
            if (isPlaying) {
                printf("Stopping playback before quitting...\n");
                stopPlayback();
            }
            printf("Exiting MP3 player.\n");
            break;
        } else {
            printf("Invalid command. Please try again.\n");
        }
    }
}
