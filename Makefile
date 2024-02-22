# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Iinclude/RudaDI -Wall -O2 -std=c++11
LDFLAGS = -L/usr/lib -lX11 # Add xLib library

# Source and build directories
SRC_DIR = src/rudadi
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
LIB_DIR = $(BUILD_DIR)

# Library name
LIB_NAME = librudadi.a

# Source files and object files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
all: $(LIB_DIR)/$(LIB_NAME)

# Rule to make the object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to make the static library
$(LIB_DIR)/$(LIB_NAME): $(OBJECTS)
	@mkdir -p $(@D)
	ar rcs $@ $(OBJECTS)

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR)
