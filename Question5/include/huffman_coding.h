#ifndef HUFFMAN_CODING_H
#define HUFFMAN_CODING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define maximum values
#define MAX_TREE_HEIGHT 1000
#define MAX_FILE_SIZE 100000

// Function prototypes
void compressFile(const char* inputFile, const char* compressedFile);
void decompressFile(const char* compressedFile, const char* decompressedFile);
void calculateFileSize(const char* fileName, long* size);

#endif // HUFFMAN_CODING_H
