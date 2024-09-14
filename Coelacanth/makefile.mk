
# ディレクトリ変数
SRC_DIR := src
LIBC_DIR := libc
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := $(OBJ_DIR)/lib

# コンパイラとフラグ
CC := gcc
CFLAGS := -Wall -O3 -march=native -mtune=native -MMD -MP
LDFLAGS := -L$(LIB_DIR) #-Wl,-rpath=$(LIB_DIR)

# ソースファイルとオブジェクトファイルの取得。LIBC_DIRにある.cファイルは再帰的に取得
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
LIBC_FILES := $(shell find $(LIBC_DIR) -type f -name "*.c")
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# 共有ライブラリの生成
LIB_SHARED := $(patsubst $(LIBC_DIR)/%.c,$(LIB_DIR)/%.so,$(LIBC_FILES))

# ビルドルール
.PHONY: all clean comp link

all: comp link

# comp: $(LIB_DIR) $(OBJ_DIR) $(LIB_SHARED) $(OBJ_FILES)
comp: $(OBJ_DIR) $(LIB_DIR) $(OBJ_FILES) $(LIB_SHARED) 

link: $(BIN_DIR) $(OBJ_FILES) $(LIB_DIR)
# $(CC) $(LDFLAGS) $(OBJ_FILES) -o $(BIN_DIR)/executable -Wl,--start-group $(LIB_SHARED) -Wl,--end-group
	$(CC) $(LDFLAGS) $(OBJ_FILES) -o $(BIN_DIR)/executable $(LIB_SHARED) 

# オブジェクトファイルの生成
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# 共有ライブラリの生成
$(LIB_DIR)/%.so: $(LIBC_DIR)/%.c
	$(CC) $(CFLAGS) -shared -fPIC $< -o $@ obj/function.o

# ディレクトリの作成
$(LIB_DIR) $(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)

-include $(OBJ_FILES:.o=.d)
