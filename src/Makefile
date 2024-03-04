CXX = g++
CXXFLAGS = -Iinclude/Ruda -Wall -O2 -std=c++11
LDFLAGS = -L/usr/lib -lX11 # Add xLib library

SUBDIRS = $(wildcard */.)
BUILD_DIR = ../build
all: $(SUBDIRS)
$(SUBDIRS):
	@mkdir -p $(BUILD_DIR)
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)


