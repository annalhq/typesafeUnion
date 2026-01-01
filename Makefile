CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
OPTFLAGS = -O3 -march=native

INCLUDE_DIR = include
TEST_DIR = test
BENCHMARK_DIR = benchmark
BUILD_DIR = build

TEST_SRC = $(TEST_DIR)/test.cpp
TEST_BIN = $(BUILD_DIR)/test

BENCHMARK_SRC = $(BENCHMARK_DIR)/benchmark.cpp
BENCHMARK_BIN = $(BUILD_DIR)/benchmark

.PHONY: all test benchmark clean

all: test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_BIN): $(TEST_SRC) $(INCLUDE_DIR)/variant.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SRC)

$(BENCHMARK_BIN): $(BENCHMARK_SRC) $(INCLUDE_DIR)/variant.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -o $@ $(BENCHMARK_SRC)

test: $(TEST_BIN)
	./$(TEST_BIN)

benchmark: $(BENCHMARK_BIN)
	./$(BENCHMARK_BIN)

clean:
	rm -rf $(BUILD_DIR)
