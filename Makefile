# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Iinclude/Ruda -Wall -O2 -std=c++11
LDFLAGS = -L/usr/lib -lX11 # Add xLib library

# Source and build directories
BUILD_DIR = build
SUBDIRS = $(wildcard */.)


all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR)
