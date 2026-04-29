CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude
LDFLAGS  = -lm

SRC_DIR  = src
OBJ_DIR  = build
BIN_DIR  = bin

COMMON_SRC = $(SRC_DIR)/operations.cpp $(SRC_DIR)/parser.cpp
MAIN_SRC   = $(SRC_DIR)/main.cpp
TEST_SRC   = $(SRC_DIR)/tests.cpp

CALC_BIN   = $(BIN_DIR)/calculator
TEST_BIN   = $(BIN_DIR)/run_tests

.PHONY: all clean test run dirs

all: dirs $(CALC_BIN) $(TEST_BIN)

dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(CALC_BIN): $(COMMON_SRC) $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Built: $@"

$(TEST_BIN): $(COMMON_SRC) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Built: $@"

test: $(TEST_BIN)
	./$(TEST_BIN)

run: $(CALC_BIN)
	./$(CALC_BIN)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
