#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common_func.h"

#define MAX_LINE_LENGTH 256

double time_spent, begin, end;
struct timespec ts1, ts2;
//way:0=clock(), 1=clock_gettime(clock_monotonic)
void begin_time()
{
	clock_gettime(CLOCK_MONOTONIC, &ts1);
}
//0=CLOCKS_PER_SEC(CPU Execution time), 1=CLOCK_MONOTONIC(elapsed time)
void end_time()
{	
	clock_gettime(CLOCK_MONOTONIC, &ts2);
	printf(" elapsed time is %f [ms]", (ts2.tv_sec - ts1.tv_sec) * 1000.0 + (ts2.tv_nsec - ts1.tv_nsec) / 1000000.0);	//milisec order
	
} 
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