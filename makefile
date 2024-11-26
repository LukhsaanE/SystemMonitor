# Compiler
CC = gcc

# Executable name
EXEC = readsystem

# Source files
SRC = main.c readsystem.c

# Default target to build the executable
$(EXEC): $(SRC)
	$(CC) -o $(EXEC) $(SRC)

# Run the program and redirect output to output.txt
run: $(EXEC)
	$(EXEC) > output.txt

# Clean up the executable and output file
clean:
	rm -f $(EXEC).exe output.txt
