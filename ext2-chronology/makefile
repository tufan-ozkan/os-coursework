# Makefile for CENG334 Homework 3
# Author: Your Name (optional)

CXX = g++

# Sources and output
SRC = histext2fs.cpp
OUT = histext2fs

# Default rule
all: $(OUT)

# Compile rule
$(OUT): $(SRC) ext2fs.h
	$(CXX) -o $(OUT) $(SRC)

# Clean rule
clean:
	rm -f $(OUT) *.o

.PHONY: all clean
