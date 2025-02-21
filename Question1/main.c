#include "mp3_player.h"
#include <dirent.h>

int main(int argc, char* argv[]) {
    SongNode* playlist = NULL;
    SongNode* currentSong = NULL;
    
    initializePlaylist(&playlist);
    
    DIR* dir = opendir("songs");
    if (!dir) {
        printf("Error: Could not open songs directory.\n");
        return 1;
    }
    
    int songCount = 0;
    char songPath[300];
    struct dirent* entry;
    
    printf("Loading songs...\n");
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".wav")) {
            sprintf(songPath, "songs/%s", entry->d_name);
            addSong(&playlist, songPath);
            songCount++;
        }
    }
    closedir(dir);
    
    if (songCount == 0) {
        printf("No songs found in the songs directory. Please add WAV files.\n");
        return 1;
    }
    
    printf("Loaded %d songs.\n", songCount);
    displayPlaylist(playlist);
    
    currentSong = playlist;
    playSong(currentSong);
    navigatePlaylist(&currentSong);  // Fixed typo: '¤tSong' → '&currentSong'
    
    SongNode* temp;
    while (playlist) {
        temp = playlist;
        playlist = playlist->next;
        free(temp);
    }
    
    return 0;
}
