# Compiler
CC := g++
# Compiler flags
CFLAGS := -Wall -Wextra -Iinclude -g
# Source and build directories
SRC_DIR := src
BIN_DIR := bin

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
# Object files
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))

# Executable
EXEC := WarehouseProject

# Build and compile
$(BIN_DIR)/$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Object files compilation
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Phony target to clean up build artifacts
.PHONY: clean
clean:
	rm -f $(BIN_DIR)/*.o $(BIN_DIR)/$(EXEC)