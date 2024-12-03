# Compiler
CC = gcc

# Executable name
EXEC = readsystem

# Source files
SRC = main.c readsystem.c

# Default target to build the executable
$(EXEC): $(SRC)
	$(CC) -fprofile-arcs -ftest-coverage -o $(EXEC) $(SRC)

# Run the program and redirect output to output.txt
run: $(EXEC)
	$(EXEC) > output.txt

# Clean up the executable and output file
clean:
	rm -f $(EXEC).exe output.txt

#This is for usage display
# Variables
CFLAGS = $(shell pkg-config --cflags gtk4)
LDFLAGS = $(shell pkg-config --libs gtk4)
TARGET = usage_display
SRC2 = usage_display.c

# Default target
all: $(TARGET)

# Compilation rule
$(TARGET): $(SRC2)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

