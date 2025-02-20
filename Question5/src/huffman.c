#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

Node* createNode(unsigned char data, int frequency) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->data = data;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!heap) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    heap->array = (Node**)malloc(capacity * sizeof(Node*));
    if (!heap->array) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void minHeapify(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->array[left]->frequency < heap->array[smallest]->frequency)
        smallest = left;
    if (right < heap->size && heap->array[right]->frequency < heap->array[smallest]->frequency)
        smallest = right;

    if (smallest != idx) {
        Node* temp = heap->array[idx];
        heap->array[idx] = heap->array[smallest];
        heap->array[smallest] = temp;
        minHeapify(heap, smallest);
    }
}

Node* extractMin(MinHeap* heap) {
    if (heap->size <= 0) return NULL;
    Node* temp = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    minHeapify(heap, 0);
    return temp;
}

void insertMinHeap(MinHeap* heap, Node* node) {
    heap->size++;
    int i = heap->size - 1;
    while (i && node->frequency < heap->array[(i - 1) / 2]->frequency) {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->array[i] = node;
}

Node* buildHuffmanTree(int freq[], int size) {
    MinHeap* heap = createMinHeap(size);
    
    for (int i = 0; i < size; i++) {
        if (freq[i] > 0) {
            heap->array[heap->size++] = createNode(i, freq[i]);
        }
    }

    for (int i = (heap->size - 1) / 2; i >= 0; i--)
        minHeapify(heap, i);

    while (heap->size > 1) {
        Node* left = extractMin(heap);
        Node* right = extractMin(heap);
        Node* parent = createNode(0, left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        insertMinHeap(heap, parent);
    }

    Node* root = extractMin(heap);
    free(heap->array);
    free(heap);
    return root;
}

void writeBits(FILE* file, unsigned int bits, int numBits) {
    static unsigned char buffer = 0;
    static int bufferBits = 0;

    while (numBits > 0) {
        buffer = (buffer << 1) | ((bits >> (numBits - 1)) & 1);
        bufferBits++;
        numBits--;

        if (bufferBits == 8) {
            fputc(buffer, file);
            buffer = 0;
            bufferBits = 0;
        }
    }
}

void flushBits(FILE* file) {
    static unsigned char buffer = 0;
    static int bufferBits = 0;

    if (bufferBits > 0) {
        buffer <<= (8 - bufferBits);
        fputc(buffer, file);
        buffer = 0;
        bufferBits = 0;
    }
}

void generateCodes(Node* node, unsigned int code, unsigned char len, unsigned int codes[], unsigned char codeLengths[]) {
    if (!node->left && !node->right) {
        codes[node->data] = code;
        codeLengths[node->data] = len;
        return;
    }
    if (node->left)
        generateCodes(node->left, (code << 1), len + 1, codes, codeLengths);
    if (node->right)
        generateCodes(node->right, (code << 1) | 1, len + 1, codes, codeLengths);
}

int compressFile(const char* inputFile) {
    FILE* in = fopen(inputFile, "rb");
    if (!in) {
        printf("Error opening input file!\n");
        return -1;
    }

    fseek(in, 0, SEEK_END);
    long fileSize = ftell(in);
    fseek(in, 0, SEEK_SET);

    FILE* out = fopen("compressed.txt", "wb");
    if (!out) {
        fclose(in);
        printf("Error creating compressed.txt!\n");
        return -1;
    }

    if (fileSize <= 8) {  // Small file: 8 bytes
        fputc(0xFF, out);  // 1-byte header
        unsigned char buffer;
        while (fread(&buffer, 1, 1, in) == 1) {
            fputc(buffer, out);
        }
    } else {  // Huffman compression
        int freq[256] = {0};
        unsigned char ch;

        while (fread(&ch, 1, 1, in) == 1)
            freq[ch]++;

        Node* root = buildHuffmanTree(freq, 256);

        unsigned int codes[256];
        unsigned char codeLengths[256];
        memset(codes, 0, sizeof(codes));
        memset(codeLengths, 0, sizeof(codeLengths));
        generateCodes(root, 0, 0, codes, codeLengths);

        // Header with size
        fputc(0x00, out);  // Marker
        int uniqueChars = 0;
        for (int i = 0; i < 256; i++)
            if (freq[i]) uniqueChars++;

        fputc(uniqueChars, out);
        unsigned short sizeShort = (unsigned short)fileSize;
        fwrite(&sizeShort, sizeof(unsigned short), 1, out);  // 2 bytes size
        for (int i = 0; i < 256; i++) {
            if (freq[i]) {
                fputc(i, out);
                fputc((unsigned char)freq[i], out);
            }
        }

        // Compress data
        fseek(in, 0, SEEK_SET);
        for (long i = 0; i < fileSize; i++) {
            fread(&ch, 1, 1, in);
            writeBits(out, codes[ch], codeLengths[ch]);
        }
        flushBits(out);

        free(root);
    }

    fclose(out);
    fclose(in);

    long compressedSize;
    calculateFileSize("compressed.txt", &compressedSize);
    if (compressedSize == -1) {
        printf("Error calculating compressed file size!\n");
        return -1;
    }

    printf("Original file size: %ld bytes\n", fileSize);
    printf("Compressed file size: %ld bytes\n", compressedSize);
    printf("Compression ratio: %.2f%%\n", (1.0 - (float)compressedSize/fileSize) * 100);

    return 0;
}

int decompressFile(const char* inputFile) {
    FILE* in = fopen(inputFile, "rb");
    if (!in) {
        printf("Error opening compressed.txt!\n");
        return -1;
    }

    FILE* out = fopen("decompressed.txt", "wb");
    if (!out) {
        fclose(in);
        printf("Error creating decompressed.txt!\n");
        return -1;
    }

    unsigned char marker = fgetc(in);

    if (marker == 0xFF) {  // Small file
        unsigned char buffer;
        int bytesRead = 0;
        while (fread(&buffer, 1, 1, in) == 1 && bytesRead < 8) {
            fputc(buffer, out);
            bytesRead++;
        }
    } else {  // Huffman compression
        int uniqueChars = fgetc(in);
        unsigned short originalSize;
        fread(&originalSize, sizeof(unsigned short), 1, in);

        int freq[256] = {0};
        unsigned char ch;
        for (int i = 0; i < uniqueChars; i++) {
            ch = fgetc(in);
            freq[ch] = fgetc(in);
        }

        Node* root = buildHuffmanTree(freq, 256);
        Node* current = root;
        unsigned char byte;
        long bytesWritten = 0;

        while (fread(&byte, 1, 1, in) == 1 && bytesWritten < originalSize) {
            for (int bitCount = 7; bitCount >= 0 && bytesWritten < originalSize; bitCount--) {
                unsigned char bit = (byte >> bitCount) & 1;
                current = bit ? current->right : current->left;
                if (!current->left && !current->right) {
                    fputc(current->data, out);
                    bytesWritten++;
                    current = root;
                }
            }
        }

        free(root);
    }

    fclose(in);
    fclose(out);
    printf("File decompressed successfully to decompressed.txt\n");
    return 0;
}

void calculateFileSize(const char* fileName, long* size) {
    FILE* file = fopen(fileName, "rb");
    if (!file) {
        *size = -1;
        return;
    }
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fclose(file);
}
