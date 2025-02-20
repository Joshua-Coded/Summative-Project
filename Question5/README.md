Below is a simplified README for your Huffman coding project, modeled after the style of the "Forensic Friend Detection System - Technical Documentation" example you provided. It focuses on the program’s functionality without delving into overly complex details, keeping it accessible and aligned with the assignment requirements for Question 5.
Huffman Coding File Compression - Technical Documentation
Project Overview
The Huffman Coding File Compression program is a tool designed to compress and decompress files using the Huffman coding algorithm, a lossless compression technique that assigns variable-length binary codes to characters based on their frequency. This project meets the requirements of Question 5 by loading any file (big or small), compressing it to compressed.txt, reporting file sizes, and decompressing it to decompressed.txt. It demonstrates practical data compression for file sharing in networking contexts.
Table of Contents
System Architecture (#system-architecture)
Algorithm Analysis (#algorithm-analysis)
Data Structures (#data-structures)
Implementation Details (#implementation-details)
Time & Space Complexity Analysis (#time--space-complexity-analysis)
User Interface (#user-interface)
Compilation & Execution (#compilation--execution)
Future Enhancements (#future-enhancements)
System Architecture
The program is built using a modular C architecture with the following components:
File Structure
huffman.h - Header file containing data structures and function prototypes (in include/).
huffman.c - Implementation of compression and decompression functions (in src/).
main.c - User interface and program control flow.
Makefile - Compilation configuration.
Core Components
File Management: Handles input file reading and output file writing.
Compression Module: Builds a Huffman tree and compresses data to compressed.txt.
Decompression Module: Reconstructs the original file into decompressed.txt.
Statistics Reporter: Displays original and compressed file sizes with the compression ratio.
Algorithm Analysis
Compression Algorithm
The compression process involves:
Function: compressFile(inputFile)
    1. If file ≤ 8 bytes:
        a. Write a 1-byte marker and copy raw data
    2. Else:
        a. Count frequency of each byte
        b. Build a Huffman tree
        c. Generate bit codes for each character
        d. Write header and compressed data to compressed.txt
Decompression Algorithm
The decompression process involves:
Function: decompressFile(inputFile)
    1. If marker indicates small file:
        a. Copy raw data after marker
    2. Else:
        a. Read header with frequency table
        b. Rebuild Huffman tree
        c. Decode bits to characters and write to decompressed.txt
Data Structures
Node Structure
c
typedef struct Node {
    unsigned char data;       // Character value
    int frequency;            // Frequency of occurrence
    struct Node* left;        // Left child
    struct Node* right;       // Right child
} Node;
Min-Heap Structure
c
typedef struct MinHeap {
    Node** array;             // Array of node pointers
    int size;                 // Current size
    int capacity;             // Maximum capacity
} MinHeap;
File Format
Small Files (≤ 8 bytes): 1-byte marker (0xFF) + raw data.
Larger Files: 1-byte marker (0x00) + 1-byte unique char count + (2 bytes per unique char: 1 char, 1 freq) + compressed data.
Implementation Details
File Processing
The program manages files as follows:
Small Files: Directly copies data with a 1-byte marker for files ≤ 8 bytes.
Larger Files: Analyzes byte frequencies, constructs a Huffman tree, and writes compressed data with a header containing frequency information.
User Interface Implementation
The interface is a simple command-line tool:
Commands: Accepts compress or decompress with a file argument.
Output: Displays file sizes and compression ratio after compression.
Time & Space Complexity Analysis
Time Complexity
Compression:
Small Files: O(n) - Linear copy of data, where n is file size.
Larger Files: O(n + k log k) - Frequency count (O(n)) + tree building (O(k log k)), where k is unique characters (max 256).
Decompression:
Small Files: O(n) - Linear copy.
Larger Files: O(m) - Bit decoding, where m is compressed size (proportional to n).
Space Complexity
Huffman Tree: O(k) - Nodes for unique characters.
Min-Heap: O(k) - Temporary storage for tree construction.
Total: O(k) - Dominated by tree and heap, where k ≤ 256.
User Interface
The program offers a straightforward command-line interface:
Commands: compress <input_file> or decompress <compressed.txt>.
Output: Reports sizes and ratio (e.g., "Original file size: 28 bytes, Compressed file size: 20 bytes, Compression ratio: 28.57%").
Example
bash
echo "joshua is the best engineer" > input.txt
./huffman compress input.txt
./huffman decompress compressed.txt
Compilation & Execution
Prerequisites
GCC compiler.
Make utility.
Compilation
bash
make
The Makefile includes:
makefile
CC = gcc
CFLAGS = -Wall -g -Iinclude
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
Execution
bash
./huffman compress input.txt
./huffman decompress compressed.txt

