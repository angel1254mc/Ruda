# Compiler and compiler flags
CXX = g++
CXXFLAGS = -I../../include/Ruda -Wall -O2 -std=c++11


BUILD_DIR = ../../build
OBJ_DIR = $(BUILD_DIR)/Util
LIB_DIR = $(BUILD_DIR)/libs
# Library name
LIBRARY = $(LIB_DIR)/librudautil.a

# Source files
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(LIBRARY)

$(BUILD_DIR):
	mkdir -p $@

$(LIBRARY): $(OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

# Rule to compile source files to object files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(LIBRARY)