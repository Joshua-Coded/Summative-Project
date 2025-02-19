# Define compiler and flags
CC = gcc
CFLAGS = -Wall -g `sdl2-config --cflags` -Iinclude
LDLIBS = `sdl2-config --libs`

# Define source files, object files, and executable
SRC_FILES = src/mp3_player.c main.c
OBJS = $(SRC_FILES:.c=.o)
EXEC = mp3_player
LDLIBS = `sdl2-config --libs` -pthread

# Default target
all: $(EXEC)

# Linking objects to create executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDLIBS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f src/*.o *.o $(EXEC)

# Run the program
run: all
	./$(EXEC)

# Test with sample songs
test:
	@echo "Testing MP3 Player..."
	@echo "Running program..."
	./$(EXEC)
