# Compiler
CC = gcc

# Executable names
EXEC = readsystem
TARGET = usage_display

# Source files for system monitor
SRC = main.c readsystem.c

# Source files for usage display
SRC2 = usage_display.c readsystem.c

# GTK Flags
CFLAGS = $(shell pkg-config --cflags gtk4)
LDFLAGS = $(shell pkg-config --libs gtk4)

# Default target to build the system monitor executable
$(EXEC): $(SRC)
	$(CC) -fprofile-arcs -ftest-coverage -o $(EXEC) $(SRC)

# Build the usage display with GTK
$(TARGET): $(SRC2)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC2) $(LDFLAGS)

# Run the system monitor and redirect output
run: $(EXEC)
	$(EXEC) > output.txt

# Clean up build artifacts
clean:
	rm -f $(EXEC).exe $(TARGET).exe output.txt *.gcda *.gcno
