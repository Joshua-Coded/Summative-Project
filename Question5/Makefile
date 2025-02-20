# Define compiler and flags
CC = gcc
CFLAGS = -Wall -g -Iinclude
LDLIBS = 

# Define source files, object files, and executable
SRC_DIR = src
INCLUDE_DIR = include
SRC_FILES = $(SRC_DIR)/huffman_coding.c main.c
OBJS = $(SRC_FILES:.c=.o)
EXEC = huffman_coding

# Default target
all: $(EXEC)

# Linking objects to create executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDLIBS)

# Compile source files into object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(SRC_DIR)/*.o *.o $(EXEC)

# Run the program
run: all
	./$(EXEC)
