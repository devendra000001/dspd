# Define the C compiler to use
CC = gcc

# Define compiler flags:
# -Wall = Show all warnings
# -g    = Include debug information
CFLAGS = -Wall -g

# Define any libraries to link
# (e.g., -lm for math library)
LDFLAGS =

# List all your .c source files
SOURCES = main.c dbManager.c dbOperations.c

# List all your object files (auto-generates from .c files)
OBJECTS = $(SOURCES:.c=.o)

# The final executable name
TARGET = my_program

# --- Rules Below ---

# Default target
all: $(TARGET)

# Link final executable
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile each .c into .o
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Declares phony targets
.PHONY: all clean
