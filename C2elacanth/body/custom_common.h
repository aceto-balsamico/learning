#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <stdarg.h>

double time_spent, begin, end;
struct timespec ts1, ts2;

extern void begin_time();
extern double end_time_us();
extern double end_time_ms();
extern void progressBar(int current, int total, int barWidth);

#define	MAX_QUEUE_SIZE 100
