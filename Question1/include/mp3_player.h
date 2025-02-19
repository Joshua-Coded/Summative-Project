#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a doubly linked list node
typedef struct SongNode {
    char songName[256];
    struct SongNode* next;
    struct SongNode* prev;
} SongNode;

// Function prototypes
void initializePlaylist(SongNode** head);
void addSong(SongNode** head, const char* songName);
void displayPlaylist(const SongNode* head);
void playSong(const SongNode* current);
void navigatePlaylist(SongNode** current);

#endif
