#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdbool.h>
#include <stdio.h>

typedef struct Node {
    unsigned char data;
    int frequency;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct MinHeap {
    Node** array;
    int size;
    int capacity;
} MinHeap;

Node* createNode(unsigned char data, int frequency);
MinHeap* createMinHeap(int capacity);
void minHeapify(MinHeap* heap, int idx);
Node* extractMin(MinHeap* heap);
void insertMinHeap(MinHeap* heap, Node* node);
Node* buildHuffmanTree(int freq[], int size);
void writeBits(FILE* file, unsigned int bits, int numBits);
void flushBits(FILE* file);
void generateCodes(Node* node, unsigned int code, unsigned char len, unsigned int codes[], unsigned char codeLengths[]);
void freeHuffmanTree(Node* root);
int compressFile(const char* inputFile);
int decompressFile(const char* inputFile);
void calculateFileSize(const char* fileName, long* size);

#endif
