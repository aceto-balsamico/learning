#pragma once

/* include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <stdarg.h>


/* Function prototypes */
extern void begin_time();
extern double end_time_us();
extern double end_time_ms();
extern void progressBar(int current, int total, int barWidth);

/* Global variables */
double time_spent, begin, end;
struct timespec ts1, ts2;

/* Macro definitions */
#define	MAX_QUEUE_SIZE 100

/* Enum definitions */
typedef enum _length
{
	LENGTH2 	= 2,
	LENGTH4		= 4,
	LENGTH8		= 8,
	LENGTH16	= 16,
	LENGTH32	= 32,	
	LENGTH64	= 64,	
	LENGTH128	= 128,	
	LENGTH256	= 256,	
	LENGTH512	= 512,	
	LENGTH1024	= 1024,	
	LENGTH2048	= 2048,	
	LENGTH4096	= 4096,	
	LENGTH8192	= 8192,

}e_LENGTH;

