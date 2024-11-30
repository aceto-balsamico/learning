# コンパイラとオプション
CC = gcc
SHELL_ARG =
INCLUDE = -I$(SRCDIR) -I$(bodyDIR)
CFLAGS = $(SHELL_ARG) -MMD -MP $(INCLUDE)
CFLAGS_SO = $(CFLAGS) -fPIC
LDFLAGS_SO = -shared

# ディレクトリ設定
OBJDIR = obj
BINDIR = bin
SRCDIR = src
bodyDIR = body
LIBDIR = dev_program
LIBOBJDIR = $(OBJDIR)/dev_program
STATIC_LIB = $(OBJDIR)/libsrc.a

# 必要なディレクトリの作成
DIRS = $(OBJDIR)/src $(OBJDIR)/body $(BINDIR)
$(shell mkdir -p $(DIRS))

# 各ディレクトリ内の.cファイルをリスト化
SRC_C_FILES = $(wildcard $(SRCDIR)/*.c)
body_C_FILES = $(wildcard $(bodyDIR)/*.c)
LIB_C_FILES = $(shell find $(LIBDIR) -name "*.c")

# オブジェクトファイルと共有ライブラリの出力ファイル指定
SRC_OBJS = $(SRC_C_FILES:$(SRCDIR)/%.c=$(OBJDIR)/src/%.o)
body_OBJS = $(body_C_FILES:$(bodyDIR)/%.c=$(OBJDIR)/body/%.o)

# LIB_C_FILESからlib接頭辞を除去したパスで.soファイル名を指定
LIB_SOS = $(patsubst $(LIBDIR)/%.c,$(LIBOBJDIR)/%.so,$(LIB_C_FILES))

# 実行ファイル
TARGET = $(BINDIR)/main_executable

# ビルド
all: $(TARGET)
	@echo "Done: $(TARGET)"

# 静的ライブラリ生成（存在しない場合のみ作成）
ifeq ($(wildcard $(STATIC_LIB)),)
$(STATIC_LIB): $(SRC_OBJS)
	ar rcs $@ $^
endif

# 実行ファイルのリンク
$(TARGET): $(body_OBJS) $(LIB_SOS) $(STATIC_LIB)
	$(CC) -o $@ $(body_OBJS) $(LIB_SOS) -L$(OBJDIR) -lsrc -Wl,-rpath,'$$ORIGIN/../$(LIBOBJDIR)'

# srcディレクトリの.cファイルをコンパイルしてオブジェクトファイルを作成
$(OBJDIR)/src/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# bodyディレクトリの.cファイルをコンパイルしてオブジェクトファイルを作成
$(OBJDIR)/body/%.o: $(bodyDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# dev_program内の.cファイルをコンパイルして共有ライブラリを作成
$(LIBOBJDIR)/%.so: $(LIBDIR)/%.c
	@mkdir -p $(dir $@)  # サブディレクトリを作成
	$(CC) $(CFLAGS_SO) $(LDFLAGS_SO) $< -o $@

# 共有ライブラリのみのビルド
lib: $(LIB_SOS)

# 静的ライブラリを残して削除
clean:
	find $(OBJDIR) -type f ! -name "*.a" -delete
	rm -rf bin/*

# cleanの短縮コマンド
cl: clean

# 全部削除
alcl:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all lib clean cl alcl

-include $(OBJ_FILES:.o=.d)