# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11 -I../../include/Ruda -Wno-unused-variable
LDFLAGS = -L../../build/libs -lrudadi -lX11 # Adjusted to point two directories up

# Name of the executable
EXEC = main

# Dependencies
DEPS = ../../src/RudaDI/structure.cpp ../../src/RudaDI/draw.cpp ../../src/RudaDI/di_window.cpp ../../src/RudaDI/init.cpp

# Source file
SOURCE = main.cpp

# Default target
all: $(EXEC)

$(EXEC): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(EXEC) $(DEPS) $(LDFLAGS)

# Clean the build
clean:
	rm -f $(EXEC)
