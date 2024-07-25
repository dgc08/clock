# Target executable
TARGET = clok

# Compiler and flags
CC = gcc
CFLAGS = -I src/ -Wall -Wextra -g #-w

SRC_DIRS = src

# The directory for object files
OBJ_DIR = tmp

# Find all .c files in SRC_DIRS
SRCS := $(shell find $(SRC_DIRS) -name '*.c')

# Generate the object file names by replacing .c with .o and prefixing with OBJ_DIR/
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Link the object files to create the target
$(TARGET): $(OBJS)
	@mkdir -p $(OBJ_DIR)  # Ensure OBJ_DIR exists
	$(CC) $(LDFLAGS) -o $@ $^

# Compile .c files into .o files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)  # Ensure the directory for the object file exists
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up the build
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
