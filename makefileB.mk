CC := gcc
CXX := g++
CFLAGS := -Wall -g
CXXFLAGS := -Wall -g
OPTIMIZE_FILE := optimize.txt
SRC_DIR := src
ANALYSIS_DIR := analysis
LIB_DIR := lib
OBJ_DIR := obj
BIN_DIR := bin

SRC_DIRS := $(SRC_DIR) $(ANALYSIS_DIR) $(LIB_DIR)
OBJ_DIRS := $(addprefix $(OBJ_DIR)/,$(SRC_DIRS))

SRC_FILES := $(wildcard $(addsuffix /*.c,$(SRC_DIRS))) $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
OPTIMIZE_FILES := $(shell cat $(OPTIMIZE_FILE))
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES)))
OBJ_FILES_OPTIMIZED := $(patsubst %.o,%_opt.o,$(addprefix $(OBJ_DIR)/,$(filter $(addprefix %/,$(OPTIMIZE_FILES)),$(OBJ_FILES))))

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
