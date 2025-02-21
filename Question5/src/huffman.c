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
        free(heap);
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

void freeHuffmanTree(Node* root) {
    if (root == NULL) return;
    
    // Recursively free left and right subtrees
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    
    // Free the current node
    free(root);
}

Node* buildHuffmanTree(int freq[], int size) {
    MinHeap* heap = createMinHeap(size);
    
    for (int i = 0; i < size; i++) {
        if (freq[i] > 0) {
            heap->array[heap->size++] = createNode(i, freq[i]);
        }
    }

    if (heap->size == 0) {
        free(heap->array);
        free(heap);
        return NULL;
    }

    if (heap->size == 1) {
        // Special case: only one character in the input
        Node* singleNode = heap->array[0];
        free(heap->array);
        free(heap);
        return singleNode;
    }

    for (int i = (heap->size - 1) / 2; i >= 0; i--)
        minHeapify(heap, i);

    while (heap->size > 1) {
        Node* left = extractMin(heap);
        Node* right = extractMin(heap);
        
        Node* parent = createNode('$', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        
        insertMinHeap(heap, parent);
    }

    Node* root = extractMin(heap);
    free(heap->array);
    free(heap);
    return root;
}

// Global variables for bit operations
static unsigned char buffer = 0;
static int bufferBits = 0;

void writeBits(FILE* file, unsigned int bits, int numBits) {
    while (numBits > 0) {
        // Extract the highest bit from 'bits'
        unsigned char bit = (bits >> (numBits - 1)) & 1;
        
        // Insert this bit to our buffer
        buffer = (buffer << 1) | bit;
        bufferBits++;
        
        // If buffer is full (8 bits), write it to file
        if (bufferBits == 8) {
            fputc(buffer, file);
            buffer = 0;
            bufferBits = 0;
        }
        
        numBits--;
    }
}

void flushBits(FILE* file) {
    if (bufferBits > 0) {
        // Pad remaining bits with zeros
        buffer <<= (8 - bufferBits);
        fputc(buffer, file);
        buffer = 0;
        bufferBits = 0;
    }
}

void generateCodes(Node* node, unsigned int code, unsigned char len, unsigned int codes[], unsigned char codeLengths[]) {
    if (node == NULL) return;
    
    if (!node->left && !node->right) {
        codes[node->data] = code;
        codeLengths[node->data] = len;
        return;
    }
    
    // Traverse left (add bit 0)
    generateCodes(node->left, (code << 1), len + 1, codes, codeLengths);
    
    // Traverse right (add bit 1)
    generateCodes(node->right, (code << 1) | 1, len + 1, codes, codeLengths);
}

int compressFile(const char* inputFile) {
    FILE* in = fopen(inputFile, "rb");
    if (!in) {
        printf("Error opening input file!\n");
        return -1;
    }

    // Get file size
    fseek(in, 0, SEEK_END);
    long fileSize = ftell(in);
    fseek(in, 0, SEEK_SET);

    if (fileSize == 0) {
        printf("Input file is empty!\n");
        fclose(in);
        return -1;
    }

    FILE* out = fopen("compressed.txt", "wb");
    if (!out) {
        fclose(in);
        printf("Error creating compressed file!\n");
        return -1;
    }

    // For very small files, use direct copy instead of Huffman
    if (fileSize <= 8) {
        fputc(0xFF, out);  // Special marker for direct copy
        unsigned char buffer;
        while (fread(&buffer, 1, 1, in) == 1) {
            fputc(buffer, out);
        }
    } else {
        // Count character frequencies
        int freq[256] = {0};
        unsigned char ch;
        
        while (fread(&ch, 1, 1, in) == 1) {
            freq[ch]++;
        }

        // Build Huffman tree
        Node* root = buildHuffmanTree(freq, 256);
        if (!root) {
            printf("Error building Huffman tree!\n");
            fclose(in);
            fclose(out);
            return -1;
        }

        // Generate Huffman codes
        unsigned int codes[256] = {0};
        unsigned char codeLengths[256] = {0};
        generateCodes(root, 0, 0, codes, codeLengths);

        // Write header - format marker
        fputc(0x00, out);
        
        // Count and write unique characters
        int uniqueChars = 0;
        for (int i = 0; i < 256; i++) {
            if (freq[i] > 0) uniqueChars++;
        }
        fputc(uniqueChars, out);
        
        // Write original file size (4 bytes)
        unsigned int size32 = (unsigned int)fileSize;
        fwrite(&size32, sizeof(unsigned int), 1, out);
        
        // Write frequency table
        for (int i = 0; i < 256; i++) {
            if (freq[i] > 0) {
                fputc(i, out);
                fwrite(&freq[i], sizeof(int), 1, out);
            }
        }

        // Reset buffer for bit operations
        buffer = 0;
        bufferBits = 0;
        
        // Compress data
        fseek(in, 0, SEEK_SET);
        while (fread(&ch, 1, 1, in) == 1) {
            writeBits(out, codes[ch], codeLengths[ch]);
        }
        
        // Write any remaining bits
        flushBits(out);
        
        // Free the Huffman tree
        freeHuffmanTree(root);
    }

    fclose(in);
    fclose(out);

    // Calculate and display compression statistics
    long compressedSize;
    calculateFileSize("compressed.txt", &compressedSize);
    if (compressedSize == -1) {
        printf("Error calculating compressed file size!\n");
        return -1;
    }

    printf("Original file size: %ld bytes\n", fileSize);
    printf("Compressed file size: %ld bytes\n", compressedSize);
    
    float ratio = (1.0 - (float)compressedSize/fileSize) * 100;
    printf("Compression ratio: %.2f%%\n", ratio);

    return 0;
}

int decompressFile(const char* inputFile) {
    FILE* in = fopen(inputFile, "rb");
    if (!in) {
        printf("Error opening compressed file!\n");
        return -1;
    }

    FILE* out = fopen("decompressed.txt", "wb");
    if (!out) {
        fclose(in);
        printf("Error creating decompressed file!\n");
        return -1;
    }

    // Read the marker byte
    unsigned char marker;
    if (fread(&marker, 1, 1, in) != 1) {
        printf("Error reading file format marker!\n");
        fclose(in);
        fclose(out);
        return -1;
    }

    if (marker == 0xFF) {  // Direct copy mode
        printf("Direct copy mode detected\n");
        unsigned char buffer;
        while (fread(&buffer, 1, 1, in) == 1) {
            fputc(buffer, out);
        }
    } else if (marker == 0x00) {  // Huffman compression
        // Read header information
        unsigned char uniqueCharsRaw;
        if (fread(&uniqueCharsRaw, 1, 1, in) != 1) {
            printf("Error reading unique character count!\n");
            fclose(in);
            fclose(out);
            return -1;
        }
        int uniqueChars = uniqueCharsRaw;
        
        // Read original file size (4 bytes)
        unsigned int originalSize;
        if (fread(&originalSize, sizeof(unsigned int), 1, in) != 1) {
            printf("Error reading original file size!\n");
            fclose(in);
            fclose(out);
            return -1;
        }
        
        printf("Decompressing file of size %u bytes with %d unique characters\n", 
               originalSize, uniqueChars);
        
        // Read frequency table
        int freq[256] = {0};
        for (int i = 0; i < uniqueChars; i++) {
            unsigned char ch;
            int frequency;
            
            if (fread(&ch, 1, 1, in) != 1 || 
                fread(&frequency, sizeof(int), 1, in) != 1) {
                printf("Error reading frequency table entry %d\n", i);
                fclose(in);
                fclose(out);
                return -1;
            }
            freq[ch] = frequency;
        }

        // Build Huffman tree
        Node* root = buildHuffmanTree(freq, 256);
        if (!root) {
            printf("Error building Huffman tree for decompression!\n");
            fclose(in);
            fclose(out);
            return -1;
        }

        // Decompress data
        Node* current = root;
        unsigned int bytesWritten = 0;
        unsigned char byte;
        
        while (bytesWritten < originalSize && fread(&byte, 1, 1, in) == 1) {
            for (int i = 7; i >= 0 && bytesWritten < originalSize; i--) {
                // Extract bit
                unsigned char bit = (byte >> i) & 1;
                
                // Navigate the Huffman tree
                current = bit ? current->right : current->left;
                
                // If we've reached a leaf node
                if (!current->left && !current->right) {
                    fputc(current->data, out);
                    bytesWritten++;
                    current = root;  // Reset to root
                    
                    // Stop if we've written all expected bytes
                    if (bytesWritten >= originalSize) {
                        break;
                    }
                }
            }
        }
        
        // Check if we decompressed the correct number of bytes
        if (bytesWritten != originalSize) {
            printf("Warning: Expected to decompress %u bytes, but got %u bytes\n", 
                  originalSize, bytesWritten);
        }
        
        // Free the Huffman tree
        freeHuffmanTree(root);
    } else {
        printf("Invalid file format marker: %02X\n", marker);
        fclose(in);
        fclose(out);
        return -1;
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
