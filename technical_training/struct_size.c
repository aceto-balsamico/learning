#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct timespec time1, time2;

void begin_time() {
	clock_gettime(CLOCK_MONOTONIC, &time1);
}

void end_time() {
	clock_gettime(CLOCK_MONOTONIC, &time2);
	printf(" elapsed time is %f [ms]", (time2.tv_sec - time1.tv_sec) * 1000.0 + (time2.tv_nsec - time1.tv_nsec) / 1000000.0);	//milisec order
}

typedef struct s {
	int a;
	int b;
	char c;
} s_DataBox;

int main() 
{
	printf("sizeof(struct s) = %lu\n", sizeof(s_DataBox));

	s_DataBox box[2];

	printf("[0]:%p, [1]:%p\n", &box[0], &box[1]);
	printf("size:%ld\n", (long)&box[1] - (long)&box[0]);

	uint8_t array1[100000];
	uint8_t array2[100000];
	int count = 100000;
	for (size_t i = 0; i < count; i++)
	{
		array1[i] = rand();
		array2[i] = 0x00;
	}
	int failbit_count = 0;
	begin_time();
	for (size_t i = 0; i < count; i++)
	{
		if (array1[i] != array2[i])
		{
			for(int bit = 1; bit <= 0xFF; bit<<=1)
			{
				int temp1 = array1[i] & bit;
				int temp2 = array2[i] & bit;
				if (temp1 != temp2)
				{
					failbit_count++;
				}
			}
		}
	}
	end_time();
	printf("failbit_count = %d\n", failbit_count);
	
	failbit_count = 0;
	begin_time();
    for (size_t i = 0; i < (count + 7) / 8; i++)
    {
        if ((*((uint64_t*)array1 + i)) != (*((uint64_t*)array2 + i)))
        {
            for (uint64_t bit = 1; bit <= 0xFFFFFFFFFFFFFFFF; bit <<= 1)
			// while(1)
            {
                uint64_t temp1 = *((uint64_t*)array1 + i) & bit;
                uint64_t temp2 = *((uint64_t*)array2 + i) & bit;
                if (temp1 != temp2)
                {
                    failbit_count++;
                }
				if(bit == 0x8000000000000000)break;
            }
        }
    }
	end_time();
	printf("failbit_count = %d\n", failbit_count);	

	return 0;
}