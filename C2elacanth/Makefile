# コンパイラとオプション
CC = gcc
SHELL_ARG =
INCLUDE = -I$(SRCDIR) -I$(ANALYSISDIR)
CFLAGS = $(SHELL_ARG) -MMD -MP $(INCLUDE)
CFLAGS_SO = $(CFLAGS) -fPIC $(INCLUDE)
LDFLAGS_SO = -shared

# ディレクトリ設定
OBJDIR = obj
BINDIR = bin
SRCDIR = src
ANALYSISDIR = analysis
LIBDIR = analysis_library
LIBOBJDIR = $(OBJDIR)/analysis_library
STATIC_LIB = $(OBJDIR)/libsrc.a

# 必要なディレクトリの作成
DIRS = $(OBJDIR)/src $(OBJDIR)/analysis $(LIBOBJDIR) $(BINDIR)
$(shell mkdir -p $(DIRS))

# 各ディレクトリ内の.cファイルをリスト化
SRC_C_FILES = $(wildcard $(SRCDIR)/*.c)
ANALYSIS_C_FILES = $(wildcard $(ANALYSISDIR)/*.c)
LIB_C_FILES = $(shell find $(LIBDIR) -name "*.c")

# オブジェクトファイルと共有ライブラリの出力ファイル指定
SRC_OBJS = $(SRC_C_FILES:$(SRCDIR)/%.c=$(OBJDIR)/src/%.o)
ANALYSIS_OBJS = $(ANALYSIS_C_FILES:$(ANALYSISDIR)/%.c=$(OBJDIR)/analysis/%.o)
LIB_SOS = $(patsubst $(LIBDIR)/%.c,$(LIBOBJDIR)/lib%.so,$(LIB_C_FILES))

# 実行ファイル
TARGET = $(BINDIR)/main_executable

# ビルド
all: $(TARGET)

# 静的ライブラリ生成（存在しない場合のみ作成）
ifeq ($(wildcard $(STATIC_LIB)),)
$(STATIC_LIB): $(SRC_OBJS)
	ar rcs $@ $^
endif

# 実行ファイルのリンク
$(TARGET): $(ANALYSIS_OBJS) $(LIB_SOS) $(STATIC_LIB)
	$(CC) -o $@ $(ANALYSIS_OBJS) -L$(LIBOBJDIR) -L$(OBJDIR) -Wl,-rpath,$(LIBOBJDIR) $(patsubst $(LIBOBJDIR)/lib%.so,-l%,$(LIB_SOS)) -lsrc

# srcディレクトリの.cファイルをコンパイルしてオブジェクトファイルを作成
$(OBJDIR)/src/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# analysisディレクトリの.cファイルをコンパイルしてオブジェクトファイルを作成
$(OBJDIR)/analysis/%.o: $(ANALYSISDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# analysis_library内の.cファイルをコンパイルして共有ライブラリを作成
$(LIBOBJDIR)/lib%.so: $(LIBDIR)/%.c
	$(CC) $(CFLAGS_SO) $(LDFLAGS_SO) $< -o $@

# 共有ライブラリのみのビルド
lib: $(LIB_SOS)

# 静的ライブラリを残して削除
clean:
	find $(OBJDIR) -type f ! -name "*.a" -delete  # .a以外のファイルを削除
	rm -rf $(BINDIR)/*  # binディレクトリ内のファイルを削除

# cleanの短縮コマンド
cl: clean

# 全部削除
alcl:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all lib clean cl alcl