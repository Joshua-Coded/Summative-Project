#include "../include/huffman_coding.h"
#include <stdbool.h>
#include <limits.h>
#include <string.h>

// Define structures for Huffman coding
typedef struct Node {
    unsigned char character;
    int frequency;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct MinHeapNode {
    Node** node;
    int size;
    int capacity;
} MinHeapNode;

// Helper structure for bit-level operations
typedef struct {
    FILE* file;
    unsigned char buffer;
    int bitCount;
} BitFile;

// Helper functions for bit-level operations
void initBitFileWrite(BitFile* bitFile, FILE* file) {
    bitFile->file = file;
    bitFile->buffer = 0;
    bitFile->bitCount = 0;
}

void writeBit(BitFile* bitFile, int bit) {
    bitFile->buffer = (bitFile->buffer << 1) | (bit & 1);
    bitFile->bitCount++;
    
    if (bitFile->bitCount == 8) {
        fputc(bitFile->buffer, bitFile->file);
        bitFile->buffer = 0;
        bitFile->bitCount = 0;
    }
}

void flushBitFile(BitFile* bitFile) {
    if (bitFile->bitCount > 0) {
        // Pad remaining bits with zeros
        bitFile->buffer = bitFile->buffer << (8 - bitFile->bitCount);
        fputc(bitFile->buffer, bitFile->file);
    }
}

void initBitFileRead(BitFile* bitFile, FILE* file) {
    bitFile->file = file;
    bitFile->buffer = 0;
    bitFile->bitCount = 0;
}

int readBit(BitFile* bitFile) {
    if (bitFile->bitCount == 0) {
        bitFile->buffer = fgetc(bitFile->file);
        if (feof(bitFile->file)) {
            return -1; // EOF
        }
        bitFile->bitCount = 8;
    }
    
    bitFile->bitCount--;
    return (bitFile->buffer >> bitFile->bitCount) & 1;
}

// Helper functions for Huffman tree
Node* newNode(unsigned char character, int frequency) {
    Node* temp = (Node*)malloc(sizeof(Node));
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    temp->character = character;
    temp->frequency = frequency;
    temp->left = temp->right = NULL;
    return temp;
}

MinHeapNode* createMinHeap(int capacity) {
    MinHeapNode* minHeap = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    if (!minHeap) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->node = (Node**)malloc(capacity * sizeof(Node*));
    if (!minHeap->node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    return minHeap;
}

void swapMinHeapNode(Node** a, Node** b) {
    Node* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeapNode* minHeap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < minHeap->size && minHeap->node[left]->frequency < minHeap->node[smallest]->frequency)
        smallest = left;

    if (right < minHeap->size && minHeap->node[right]->frequency < minHeap->node[smallest]->frequency)
        smallest = right;

    if (smallest != index) {
        swapMinHeapNode(&minHeap->node[smallest], &minHeap->node[index]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(MinHeapNode* minHeap) {
    return (minHeap->size == 1);
}

Node* extractMin(MinHeapNode* minHeap) {
    Node* temp = minHeap->node[0];
    minHeap->node[0] = minHeap->node[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeapNode* minHeap, Node* minHeapNode) {
    int i = minHeap->size++;
    while (i && minHeapNode->frequency < minHeap->node[(i - 1) / 2]->frequency) {
        minHeap->node[i] = minHeap->node[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->node[i] = minHeapNode;
}

void buildMinHeap(MinHeapNode* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// Structure to store character codes
typedef struct {
    unsigned char character;
    unsigned int code;
    unsigned int codeBits;
} CharacterCode;

// Function to generate character codes
void generateCodes(Node* root, unsigned int code, unsigned int codeBits, CharacterCode codes[], int* index) {
    if (root->left) {
        generateCodes(root->left, (code << 1), codeBits + 1, codes, index);
    }

    if (root->right) {
        generateCodes(root->right, (code << 1) | 1, codeBits + 1, codes, index);
    }

    if (!root->left && !root->right) {
        codes[*index].character = root->character;
        codes[*index].code = code;
        codes[*index].codeBits = codeBits;
        (*index)++;
    }
}

// Build Huffman Tree
Node* buildHuffmanTree(unsigned char data[], int freq[], int size) {
    MinHeapNode* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->node[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    while (!isSizeOne(minHeap)) {
        Node* left = extractMin(minHeap);
        Node* right = extractMin(minHeap);

        Node* top = newNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

// Free the Huffman Tree
void freeHuffmanTree(Node* root) {
    if (root) {
        freeHuffmanTree(root->left);
        freeHuffmanTree(root->right);
        free(root);
    }
}

// Compress file implementation with bit-level operations
void compressFile(const char* inputFile, const char* compressedFile) {
    FILE* in = fopen(inputFile, "rb");
    FILE* out = fopen(compressedFile, "wb");

    if (!in || !out) {
        printf("Error opening files.\n");
        return;
    }

    // Calculate frequency of each character
    int freq[256] = {0};
    unsigned char ch;
    while (fread(&ch, 1, 1, in) == 1)
        freq[ch]++;

    fclose(in);

    // Count non-zero frequency characters
    int uniqueChars = 0;
    for (int i = 0; i < 256; i++)
        if (freq[i]) uniqueChars++;

    if (uniqueChars == 0) {
        printf("Empty input file.\n");
        fclose(out);
        return;
    }

    // Build frequency table
    unsigned char data[uniqueChars];
    int freqValues[uniqueChars];
    int index = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            data[index] = i;
            freqValues[index] = freq[i];
            index++;
        }
    }

    // Save file header (format: 4-byte magic number, 4-byte file size, 1-byte unique chars count)
    const unsigned char MAGIC_NUMBER[4] = {'H', 'U', 'F', 'F'};
    fwrite(MAGIC_NUMBER, 1, 4, out);
    
    long originalSize;
    calculateFileSize(inputFile, &originalSize);
    fwrite(&originalSize, sizeof(long), 1, out);
    
    fwrite(&uniqueChars, sizeof(int), 1, out);

    // Write frequency table
    for (int i = 0; i < uniqueChars; i++) {
        fwrite(&data[i], sizeof(unsigned char), 1, out);
        fwrite(&freqValues[i], sizeof(int), 1, out);
    }

    // Build Huffman tree and generate codes
    Node* root = buildHuffmanTree(data, freqValues, uniqueChars);
    CharacterCode codes[256];
    int codeIndex = 0;
    generateCodes(root, 0, 0, codes, &codeIndex);

    // Create lookup table for fast encoding
    CharacterCode lookupTable[256];
    memset(lookupTable, 0, sizeof(lookupTable));
    for (int i = 0; i < codeIndex; i++) {
        lookupTable[codes[i].character] = codes[i];
    }

    // Write compressed data using bit-level operations
    BitFile bitFile;
    initBitFileWrite(&bitFile, out);
    
    in = fopen(inputFile, "rb");
    while (fread(&ch, 1, 1, in) == 1) {
        CharacterCode code = lookupTable[ch];
        for (unsigned int i = code.codeBits; i > 0; i--) {
            int bit = (code.code >> (i-1)) & 1;
            writeBit(&bitFile, bit);
        }
    }
    
    flushBitFile(&bitFile);
    freeHuffmanTree(root);
    fclose(in);
    fclose(out);
}

// Decompress file implementation with bit-level operations
void decompressFile(const char* compressedFile, const char* decompressedFile) {
    FILE* in = fopen(compressedFile, "rb");
    FILE* out = fopen(decompressedFile, "wb");

    if (!in || !out) {
        printf("Error opening files.\n");
        return;
    }

    // Validate magic number
    unsigned char magicNumber[4];
    if (fread(magicNumber, 1, 4, in) != 4 ||
        magicNumber[0] != 'H' || magicNumber[1] != 'U' || 
        magicNumber[2] != 'F' || magicNumber[3] != 'F') {
        printf("Invalid compressed file format.\n");
        fclose(in);
        fclose(out);
        return;
    }

    // Read original file size
    long originalSize;
    if (fread(&originalSize, sizeof(long), 1, in) != 1) {
        printf("Error reading compressed file header.\n");
        fclose(in);
        fclose(out);
        return;
    }

    // Read number of unique characters
    int uniqueChars;
    if (fread(&uniqueChars, sizeof(int), 1, in) != 1) {
        printf("Error reading compressed file header.\n");
        fclose(in);
        fclose(out);
        return;
    }

    // Read frequency table
    unsigned char data[uniqueChars];
    int freq[uniqueChars];
    for (int i = 0; i < uniqueChars; i++) {
        if (fread(&data[i], sizeof(unsigned char), 1, in) != 1 ||
            fread(&freq[i], sizeof(int), 1, in) != 1) {
            printf("Error reading frequency table.\n");
            fclose(in);
            fclose(out);
            return;
        }
    }

    // Rebuild Huffman tree
    Node* root = buildHuffmanTree(data, freq, uniqueChars);
    
    // Decompress data bit by bit
    BitFile bitFile;
    initBitFileRead(&bitFile, in);
    
    long bytesWritten = 0;
    Node* current = root;
    
    while (bytesWritten < originalSize) {
        int bit = readBit(&bitFile);
        if (bit == -1) {
            printf("Unexpected end of compressed file.\n");
            break;
        }
        
        if (bit == 0) {
            current = current->left;
        } else {
            current = current->right;
        }
        
        // If we reach a leaf node, write the character
        if (!current->left && !current->right) {
            fwrite(&current->character, sizeof(unsigned char), 1, out);
            bytesWritten++;
            current = root; // Reset to the root
        }
    }
    
    freeHuffmanTree(root);
    fclose(in);
    fclose(out);
}

// Calculate file size
void calculateFileSize(const char* fileName, long* size) {
    FILE* file = fopen(fileName, "rb");
    if (!file) {
        printf("Error opening file: %s\n", fileName);
        *size = -1;
        return;
    }

    fseek(file, 0L, SEEK_END);
    *size = ftell(file);
    fclose(file);
}
