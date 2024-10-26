# Compiler and flags
CC = g++
CFLAGS = -lgdi32 -Wall -g

# Target executable
TARGET = main.exe

# Source files
SRC = main.cpp

# Default rule to build and run the program
all: clean $(TARGET) run

# Rule to build the executable
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

# Rule to run the program
run: $(TARGET)
	./$(TARGET)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)
