CC = gcc
CFLAGS = -Wall -g -Iinclude
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Create object directory if it doesn't exist
$(shell mkdir -p $(OBJ_DIR))

all: huffman

huffman: $(OBJ_DIR)/main.o $(OBJ_DIR)/huffman.o
	$(CC) $(CFLAGS) -o huffman $(OBJ_DIR)/main.o $(OBJ_DIR)/huffman.o

$(OBJ_DIR)/main.o: main.c $(INC_DIR)/huffman.h
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/huffman.o: $(SRC_DIR)/huffman.c $(INC_DIR)/huffman.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/huffman.c -o $(OBJ_DIR)/huffman.o

clean:
	rm -rf $(OBJ_DIR) huffman
