# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Target executable
TARGET = raytracer

# Source files
SRCS = main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
    $(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

.cpp.o:
    $(CXX) $(CXXFLAGS) -c $<  -o $@

clean:
    $(RM) $(OBJS) $(TARGET)

.PHONY: all clean

# End of Makefile
```