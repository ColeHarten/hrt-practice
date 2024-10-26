# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Target executable
TARGET = test

# Source files (all .cc files in the directory)
SRC = $(wildcard *.cc)

# Object files (optional, if you want to separate compilation)
OBJ = $(SRC:.cc=.o)

# Check if sanitizer flag is enabled
ifdef SAN
CXXFLAGS += -fsanitize=address,undefined
endif

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
