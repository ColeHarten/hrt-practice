# Makefile for matrix.cc

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Target executable
TARGET = test

# Source files
SRC = test.cc

# Object files (optional, if you want to separate compilation)
OBJ = $(SRC:.cc=.o)

# Default rule to build the executable
all: $(TARGET)

# Rule to compile the target executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Rule to clean up the build files
clean:
	rm -f $(TARGET) $(OBJ)

# Phony targets to prevent file name conflicts
.PHONY: all clean
