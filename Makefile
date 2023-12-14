# Makefile

# Compiler settings
CC=g++
CFLAGS=-fopenmp

# Target
TARGET=main

# Source files
SRC=main.c

# Build rule
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Clean rule
clean:
	rm -f $(TARGET)
