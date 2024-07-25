##
#
#
# @file
# @version 0.1

# Compiler
CXX = cc

# Compiler flags
CXXFLAGS = -w -Wall -Wextra -g

# Source directory
SRC_DIR = src

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object directory
OBJ_DIR = obj

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Executable name
TARGET = clok

# Default target
all: $(TARGET)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files into the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Clean up object files and executable
clean:
	rm -rf $(OBJ_DIR)/* $(TARGET)
