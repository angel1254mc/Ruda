# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11 -I../../include/Ruda -Wno-unused-variable
LDFLAGS = -L../../build/libs -lrudadi -lX11 # Adjusted to point two directories up

BUILD_DIR = ../../build

DEMO_DIR = $(BUILD_DIR)/demos
# Name of the executable
EXEC = $(DEMO_DIR)/simple-window

# Source file
SOURCE = main.cpp

# Default target
all: $(EXEC)

$(EXEC): $(SOURCE)
	@mkdir -p $(DEMO_DIR)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(EXEC) $(LDFLAGS)

# Clean the build
clean:
	rm -f $(EXEC)
