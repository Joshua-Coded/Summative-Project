#include <stdio.h>
#include <string.h>
#include "huffman.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <compress/decompress> <input_file>\n", argv[0]);
        return 1;
    }
    
    if (strcmp(argv[1], "compress") == 0) {
        if (compressFile(argv[2]) == 0) {
            return 0;
        }
    } else if (strcmp(argv[1], "decompress") == 0) {
        if (decompressFile(argv[2]) == 0) {
            return 0;
        }
    } else {
        printf("Invalid command. Use 'compress' or 'decompress'\n");
    }
    
    return 1;
}
