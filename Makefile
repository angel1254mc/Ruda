# Compiler settings - Can be customized.
CXX = g++
CXXFLAGS = -Iinclude -Wall -Wextra -std=c++11

# Project structure
SRCDIR = src/RudaDI
INCDIR = include/Ruda/RudaDI
BUILDDIR = build
TARGET = $(BUILDDIR)/librudadi.a

# Find all CPP files in the SRCDIR
SRC = $(wildcard $(SRCDIR)/*.cpp)

# Object files correspond to SRC
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

# Default make
all: $(TARGET)

# Static library creation
$(TARGET): $(OBJ)
	ar rcs $@ $^

# Compile each CPP file
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILDDIR)
	

