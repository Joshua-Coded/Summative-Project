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
    if (currentDeviceId == 0) {
        printf("Failed to open audio device: %s\n", SDL_GetError());
        SDL_FreeWAV(wavBuffer);
        SDL_Quit();
        return NULL;
    }

    SDL_QueueAudio(currentDeviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(currentDeviceId, 0);  // Start playing

    while (!shouldStop && SDL_GetQueuedAudioSize(currentDeviceId) > 0) {
        if (isPaused) {
            SDL_PauseAudioDevice(currentDeviceId, 1);  // Pause
        } else {
            SDL_PauseAudioDevice(currentDeviceId, 0);  // Resume
        }
        SDL_Delay(100);  // Poll every 100ms
    }

    // Clean up
    SDL_CloseAudioDevice(currentDeviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();
    
    isPlaying = false;  // Reset flag when done
    return NULL;
}

// Initialize playlist
void initializePlaylist(SongNode** head) {
    *head = NULL;
}

// Add song to playlist
void addSong(SongNode** head, const char* songName) {
    SongNode* newNode = (SongNode*)malloc(sizeof(SongNode));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        return;
    }
    
    strncpy(newNode->songName, songName, 255);
    newNode->songName[255] = '\0';
    newNode->next = NULL;
    
    if (*head == NULL) {
        newNode->prev = NULL;
        *head = newNode;
        return;
    }
    
    SongNode* current = *head;
    while (current->next) {
        current = current->next;
    }
    current->next = newNode;
    newNode->prev = current;
}

// Display playlist
void displayPlaylist(const SongNode* head) {
    if (!head) {
        printf("Playlist is empty.\n");
        return;
    }
    
    printf("\n===== Playlist =====\n");
    int songNumber = 1;
    const SongNode* current = head;
    while (current) {
        printf("%d. %s\n", songNumber++, current->songName);
        current = current->next;
    }
    printf("====================\n");
}

// Stop playback
void stopPlayback() {
    if (isPlaying) {
        shouldStop = true;
        if (currentDeviceId > 0) {
            SDL_ClearQueuedAudio(currentDeviceId);      // Clear audio queue
            SDL_PauseAudioDevice(currentDeviceId, 1);   // Pause device
            SDL_CloseAudioDevice(currentDeviceId);      // Close device immediately
            currentDeviceId = 0;
        }
        pthread_join(playbackThread, NULL);  // Wait for thread to finish
        shouldStop = false;
        isPlaying = false;
        isPaused = false;
    }
}

// Play a song
void playSong(const SongNode* current) {
    if (!current) {
        printf("No song to play.\n");
        return;
    }

    stopPlayback();  // Stop any ongoing playback
    
    printf("Now playing: %s\n", current->songName);
    isPlaying = true;
    isPaused = false;
    shouldStop = false;
    
    if (pthread_create(&playbackThread, NULL, playAudioThread, (void*)current) != 0) {
        printf("Failed to create playback thread.\n");
        isPlaying = false;
        return;
    }
    // Note: Thread is NOT detached, so we can join it in stopPlayback()
}

// Navigate playlist
void navigatePlaylist(SongNode** current) {
    char command[10];
    printf("\nControls: n: next, p: previous, s: stop, q: quit, x: pause/resume\n");
    
    while (1) {
        printf("\nEnter command: ");
        scanf("%s", command);

        if (strcmp(command, "n") == 0) {
            if ((*current)->next) {
                *current = (*current)->next;
                playSong(*current);
            } else {
                printf("End of playlist reached.\n");
            }
        } else if (strcmp(command, "p") == 0) {
            if ((*current)->prev) {
                *current = (*current)->prev;
                playSong(*current);
            } else {
                printf("Beginning of playlist reached.\n");
            }
        } else if (strcmp(command, "s") == 0) {
            if (isPlaying) {
                printf("Stopping playback...\n");
                stopPlayback();
                printf("Stopped.\n");
            } else {
                printf("Playback already stopped.\n");
            }
        } else if (strcmp(command, "x") == 0) {
            if (isPlaying) {
                isPaused = !isPaused;
                printf(isPaused ? "Playback paused.\n" : "Playback resumed.\n");
            } else {
                printf("No song is currently playing.\n");
            }
        } else if (strcmp(command, "q") == 0) {
            if (isPlaying) {
                printf("Stopping playback before quitting...\n");
                stopPlayback();
            }
            printf("Exiting MP3 player.\n");
            break;
        } else {
            printf("Invalid command.\n");
        }
    }
}
