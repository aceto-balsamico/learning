#ifndef CSV_UTILS_H
#define CSV_UTILS_H

#include <stdio.h>

// CSV特化のfopenとして使用する関数。秒を含めた現在時刻をファイル名に付与する。
extern FILE* create_csv_with_timestamp(const char* base_filename, char* filename, size_t filename_size);

// 指定した行数の文字列を取得する関数。
extern char* get_line(FILE* fp, size_t line_number);

// カンマ区切りの文字列配列から特定の数値n番目のトークンを取得する関数。10進数用。
extern int get_nth_token_as_int(const char* str, int n);

// カンマ区切りの文字列配列から特定の数値n番目のトークンを取得する関数。16進数用。
extern int get_nth_token_as_hex(const char* str, int n);

// カンマ区切りの文字列配列から特定の数値n番目のトークンを取得する関数。文字列用。
extern char* get_nth_token_as_string(const char* str, int n, char* buffer, size_t buffer_size);

// 特定の列のみを抽出して新規CSVとして出力する関数。
extern void extract_column_to_new_csv(const char* input_filename, const char* output_filename, int column_number);

// 2つのCSVファイルを横に連結する関数。
extern void concatenate_csv_files(const char* file1, const char* file2, const char* output_filename);
extern void concatenate_csv_horizontally(const char* fileA, const char* fileB, const char* outputFile);


#endif // CSV_UTILS_H

