CC := gcc
CXX := g++
CFLAGS := -Wall -g
CXXFLAGS := -Wall -g
SRC_DIR := src
ANALYSIS_DIR := analysis
LIB_DIR := lib
OBJ_DIR := obj
BIN_DIR := bin
OPTIMIZE_DIR := $(ANALYSIS_DIR)  # Specify the directory to apply optimization if no optimize.txt file is present

SRC_DIRS := $(SRC_DIR) $(ANALYSIS_DIR) $(LIB_DIR)
OBJ_DIRS := $(addprefix $(OBJ_DIR)/,$(SRC_DIRS))

SRC_FILES := $(wildcard $(addsuffix /*.c,$(SRC_DIRS))) $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES)))
OPTIMIZE_FILES := $(wildcard $(addsuffix /*,$(OPTIMIZE_DIR)))
OBJ_FILES_OPTIMIZED := $(patsubst %.o,$(OBJ_DIR)/%_opt.o,$(filter $(addprefix $(OBJ_DIR)/,$(OBJ_FILES))))

$(shell mkdir -p $(OBJ_DIRS) $(BIN_DIR))

$(OBJ_DIRS)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIRS)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJ_FILES_OPTIMIZED): %_opt.o: %.o
	$(CXX) $(CXXFLAGS) -o $@ -c $< -O3 -march=native

$(BIN_DIR)/output: $(OBJ_FILES_OPTIMIZED) $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
