# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude

# Project structure
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Platform-specific directory creation
ifeq ($(OS),Windows_NT)
	MKDIR_P = if not exist $(subst /,\\,$(1)) mkdir $(subst /,\\,$(1))
else
	MKDIR_P = mkdir -p $(1)
endif


# Files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TARGET := $(BIN_DIR)/hello

# Default target
all: $(TARGET)

# Link object files into binary
$(TARGET): $(OBJS)
	$(call MKDIR_P,$(BIN_DIR))
	$(CXX) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(call MKDIR_P,$(OBJ_DIR))
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Run the binary
run: all
	./$(TARGET)

# Clean build files
clean:
ifeq ($(OS),Windows_NT)
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)
else
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
endif

# Phony targets
.PHONY: all run clean
