#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_func.h"

#define MAX_LINE_LENGTH 256


void extract_lines(const char *filename, const char *start_str, const char *end_str, int start_line_offset, int line_count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int current_line = 0;
    int extract = 0;
    int start_line = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        current_line++;
        if (!extract && strstr(line, start_str) != NULL) {
            extract = 1;
            start_line = current_line + start_line_offset;
            continue;
        }
        if (extract && strstr(line, end_str) != NULL) {
            break;
        }
        if (extract && current_line >= start_line && current_line < start_line + line_count) {
            printf("%s", line);
        }
    }

    fclose(file);
}