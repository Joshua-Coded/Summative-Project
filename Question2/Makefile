# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -Iinclude
LDLIBS = 

# Define source files, object files, and executable
SRC_FILES = src/spell_checker.c main.c
OBJS = $(SRC_FILES:.c=.o)
EXEC = spell_checker

# Default target
all: $(EXEC)

# Build executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean generated files
clean:
	rm -f $(OBJS) $(EXEC)

