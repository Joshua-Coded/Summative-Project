# Define compiler and flags
CC = gcc
CFLAGS = -Wall -g -Iinclude
LDLIBS =

# Define source files, object files, and executable
SRC_FILES = src/friend_detection.c main.c
OBJS = $(SRC_FILES:.c=.o)
EXEC = friend_detection

# Default target
all: $(EXEC)

# Linking objects to create executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDLIBS)

# Compile source files into object files
%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f src/*.o *.o $(EXEC)

# Run the program
run: all
	./$(EXEC)
