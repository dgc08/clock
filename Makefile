TARGET = clok
TARGET_DEBUG = clok_debug

# Compiler and flags
CC = gcc
CFLAGS = -I src/ -Wall -Wextra

DEBUG_LDFLAGS = -fsanitize=address -static-libasan

DEBUG_FLAGS = -fsanitize=address -static-libasan -g
RELEASE_FLAGS = -O2

SRC_DIRS = src

# The directory for object files
TMP_DIR = tmp
OBJ_DIR_RELEASE = $(TMP_DIR)/release
OBJ_DIR_DEBUG = $(TMP_DIR)/debug

# Find all .c files in SRC_DIRS
SRCS := $(shell find $(SRC_DIRS) -name '*.c')

# Generate the object file names by replacing .c with .o and prefixing with OBJ_DIR/
OBJS_RELEASE := $(patsubst %.c,$(OBJ_DIR_RELEASE)/%.o,$(SRCS))
OBJS_DEBUG := $(patsubst %.c,$(OBJ_DIR_DEBUG)/%.o,$(SRCS))

# Default target
all: release

.PHONY: all debug release clean

debug: $(TARGET_DEBUG)

release: $(TARGET)

$(TARGET): $(OBJS_RELEASE)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET_DEBUG): $(OBJS_DEBUG)
	$(CC) $(LDFLAGS) $(DEBUG_LDFLAGS) -o $@ $^

# Compile .c files into .o files for release
$(OBJ_DIR_RELEASE)/%.o: %.c
	@mkdir -p $(dir $@)  # Ensure the directory for the object file exists
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c -o $@ $<

# Compile .c files into .o files for debug
$(OBJ_DIR_DEBUG)/%.o: %.c
	@mkdir -p $(dir $@)  # Ensure the directory for the object file exists
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c -o $@ $<

# Clean up the build
clean:
	rm -rf $(TMP_DIR) $(TARGET) $(TARGET_DEBUG)
