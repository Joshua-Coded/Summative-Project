#include "mp3_player.h"
#include <dirent.h>

int main(int argc, char* argv[]) {
    SongNode* playlist = NULL;
    SongNode* currentSong = NULL;
    
    // Initialize the playlist
    initializePlaylist(&playlist);
    
    // Load songs from the songs directory
    DIR* dir;
    struct dirent* entry;
    dir = opendir("songs");
    
    if (dir == NULL) {
        printf("Error: Could not open songs directory.\n");
        return 1;
    }
    
    int songCount = 0;
    char songPath[300];
    
    printf("Loading songs...\n");
    while ((entry = readdir(dir)) != NULL) {
        // Only add WAV files
        if (strstr(entry->d_name, ".wav") != NULL) {
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
    
    // Display the playlist
    displayPlaylist(playlist);
    
    // Start with the first song
    currentSong = playlist;
    
    // Play the first song and navigate through playlist
    playSong(currentSong);
    navigatePlaylist(&currentSong);
    
    // Free memory used by playlist before exiting
    SongNode* temp;
    while (playlist != NULL) {
        temp = playlist;
        playlist = playlist->next;
        free(temp);
    }
    
    return 0;
}
