# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Iinclude
BUILD_DIR := build
TEST_LDFLAGS := -lCatch2Main -lCatch2

# Source files
SRC := $(wildcard src/*.cc)
SRC_ENGINE := $(wildcard src/*.cc) # engine files
SRC_TEST := $(filter-out src/main.cc,$(SRC))  # engine files for tests (exclude main.cc)
TEST_SRC := $(wildcard test/*.cc)      # Catch2 tests + test main

# Object files
OBJ_ENGINE := $(patsubst %.cc,$(BUILD_DIR)/%.o,$(SRC_ENGINE))
OBJ_TEST := $(patsubst %.cc,$(BUILD_DIR)/%.o,$(SRC_TEST) $(TEST_SRC))

# Executables
ENGINE_EXE := $(BUILD_DIR)/lightknight
TEST_EXE := $(BUILD_DIR)/test_board

# Default target: build engine and tests
all: $(ENGINE_EXE) $(TEST_EXE)

# Compile .cc -> .o (create subdirs automatically)
$(BUILD_DIR)/%.o: %.cc
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link engine
$(ENGINE_EXE): $(OBJ_ENGINE)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Link tests
$(TEST_EXE): $(OBJ_TEST)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(TEST_LDFLAGS)

# Run tests
test: $(TEST_EXE)
	./$(TEST_EXE)

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)
