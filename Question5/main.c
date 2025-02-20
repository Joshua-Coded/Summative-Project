#include "include/huffman_coding.h"
#include <stdio.h>
#include <string.h>

void printUsage() {
    printf("Huffman Coding File Compression Tool\n");
    printf("===================================\n");
    printf("Usage:\n");
    printf("  ./huffman_coding compress <input_file>\n");
    printf("  ./huffman_coding decompress <compressed_file>\n");
    printf("  ./huffman_coding info <compressed_file>\n");
    printf("\n");
    printf("Examples:\n");
    printf("  ./huffman_coding compress myfile.txt       # Compresses to compressed.txt\n");
    printf("  ./huffman_coding decompress compressed.txt # Decompresses to decompressed.txt\n");
    printf("  ./huffman_coding info compressed.txt       # Shows compression statistics\n");
}

int main(int argc, char* argv[]) {
    const char* compressedFile = "compressed.txt";
    const char* decompressedFile = "decompressed.txt";
    
    printf("Huffman Coding Tool\n");
    printf("==================\n");

    // Process command line arguments
    if (argc < 2) {
        // Default behavior with no arguments
        const char* inputFile = "input.txt";
        printf("No arguments provided. Using default input file: %s\n", inputFile);
        
        // Compress the file
        compressFile(inputFile, compressedFile);
        
        // Calculate file sizes
        long originalSize, compressedSize;
        calculateFileSize(inputFile, &originalSize);
        calculateFileSize(compressedFile, &compressedSize);
        
        if (originalSize == -1 || compressedSize == -1) {
            printf("Error calculating file sizes.\n");
            return 1;
        }
        
        printf("Original File Size: %ld bytes\n", originalSize);
        printf("Compressed File Size: %ld bytes\n", compressedSize);
        
        // Calculate compression ratio
        double compressionRatio = ((double)(originalSize - compressedSize) / originalSize) * 100;
        printf("Compression Ratio: %.2f%%\n", compressionRatio);
        
        // Decompress the file
        decompressFile(compressedFile, decompressedFile);
        
        // Calculate decompressed file size
        long decompressedSize;
        calculateFileSize(decompressedFile, &decompressedSize);
        
        if (decompressedSize == -1) {
            printf("Error calculating decompressed file size.\n");
            return 1;
        }
        
        printf("Decompressed File Size: %ld bytes\n", decompressedSize);
    } 
    else if (strcmp(argv[1], "compress") == 0) {
        if (argc < 3) {
            printf("Error: Input file not specified.\n");
            printUsage();
            return 1;
        }
        
        const char* inputFile = argv[2];
        printf("Compressing file: %s\n", inputFile);
        
        compressFile(inputFile, compressedFile);
        
        // Calculate file sizes
        long originalSize, compressedSize;
        calculateFileSize(inputFile, &originalSize);
        calculateFileSize(compressedFile, &compressedSize);
        
        if (originalSize == -1 || compressedSize == -1) {
            printf("Error calculating file sizes.\n");
            return 1;
        }
        
        printf("Original File Size: %ld bytes\n", originalSize);
        printf("Compressed File Size: %ld bytes\n", compressedSize);
        
        // Calculate compression ratio
        double compressionRatio = ((double)(originalSize - compressedSize) / originalSize) * 100;
        printf("Compression Ratio: %.2f%%\n", compressionRatio);
        printf("Compressed file saved as: %s\n", compressedFile);
    } 
    else if (strcmp(argv[1], "decompress") == 0) {
        if (argc < 3) {
            printf("Error: Compressed file not specified.\n");
            printUsage();
            return 1;
        }
        
        const char* inputFile = argv[2];
        printf("Decompressing file: %s\n", inputFile);
        
        decompressFile(inputFile, decompressedFile);
        printf("Decompressed file saved as: %s\n", decompressedFile);
    } 
    else if (strcmp(argv[1], "info") == 0) {
        if (argc < 3) {
            printf("Error: Compressed file not specified.\n");
            printUsage();
            return 1;
        }
        
        const char* inputFile = argv[2];
        printf("Analyzing compressed file: %s\n", inputFile);
        
        // Open the compressed file and validate format
        FILE* in = fopen(inputFile, "rb");
        if (!in) {
            printf("Error: Cannot open file %s\n", inputFile);
            return 1;
        }
        
        // Check magic number
        unsigned char magicNumber[4];
        if (fread(magicNumber, 1, 4, in) != 4 ||
            magicNumber[0] != 'H' || magicNumber[1] != 'U' || 
            magicNumber[2] != 'F' || magicNumber[3] != 'F') {
            printf("Invalid compressed file format.\n");
            fclose(in);
            return 1;
        }
        
        // Read original file size
        long originalSize;
        if (fread(&originalSize, sizeof(long), 1, in) != 1) {
            printf("Error reading compressed file header.\n");
            fclose(in);
            return 1;
        }
        
        // Read unique chars count
        int uniqueChars;
        if (fread(&uniqueChars, sizeof(int), 1, in) != 1) {
            printf("Error reading compressed file header.\n");
            fclose(in);
            return 1;
        }
        
        long compressedSize;
        calculateFileSize(inputFile, &compressedSize);
        
        printf("File Analysis Results:\n");
        printf("----------------------\n");
        printf("Original File Size: %ld bytes\n", originalSize);
        printf("Compressed File Size: %ld bytes\n", compressedSize);
        printf("Unique Characters: %d\n", uniqueChars);
        
        // Calculate compression ratio
        double compressionRatio = ((double)(originalSize - compressedSize) / originalSize) * 100;
        printf("Compression Ratio: %.2f%%\n", compressionRatio);
        
        fclose(in);
    } 
    else if (strcmp(argv[1], "help") == 0) {
        printUsage();
    } 
    else {
        printf("Unknown command: %s\n", argv[1]);
        printUsage();
        return 1;
    }
    
    return 0;
}
