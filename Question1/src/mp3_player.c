#include "mp3_player.h"

// Initialize the playlist as an empty doubly linked list
void initializePlaylist(SongNode** head) {
    *head = NULL;
}

// Add a song to the playlist
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

// Display the entire playlist
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

// Simulate playing a song
void playSong(const SongNode* current) {
    if (current == NULL) {
        printf("No song to play.\n");
        return;
    }
    printf("Now playing: %s\n", current->songName);
}

// Navigate through the playlist
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
