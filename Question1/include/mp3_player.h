#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

typedef struct SongNode {
    char songName[256];
    struct SongNode* next;
    struct SongNode* prev;
} SongNode;

void initializePlaylist(SongNode** head);
void addSong(SongNode** head, const char* songName);
void displayPlaylist(const SongNode* head);
void playSong(const SongNode* current);
void navigatePlaylist(SongNode** current);

#endif // MP3_PLAYER_H
