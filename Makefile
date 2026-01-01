CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude

INCLUDE_DIR = include
TEST_DIR = test
BUILD_DIR = build

TEST_SRC = $(TEST_DIR)/test.cpp
TEST_BIN = $(BUILD_DIR)/test

.PHONY: all test clean

all: test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_BIN): $(TEST_SRC) $(INCLUDE_DIR)/variant.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SRC)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR)
