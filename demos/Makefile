CXX = g++
CXXFLAGS = -Iinclude/Ruda -Wall -O2 -std=c++11
LDFLAGS = -L/usr/lib -lX11 # Add xLib library

SUBDIRS = $(wildcard */.)

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)


