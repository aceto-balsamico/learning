#include "common.h"

#define DIM1 10
#define DIM2 10
#define DIM3 10
#define DIM4 10
#define DIM5 10

int main() 
{
    size_t dim1 = DIM1;
    size_t dim2 = DIM2;
    size_t dim3 = DIM3;
    size_t dim4 = DIM4;
	size_t dim5 = DIM5;

    size_t alignment = 64;  
	int (*array)[dim2][dim3][dim4][dim5];

	printf("How does it aligned memory?\n");
	printf("[0]posix_memalign   [1]align_alloc   [2]malloc   [3]calloc\n");

	int way_to_align = 0;
	scanf("%d", &way_to_align);
	if(way_to_align == 0)
	{
		if(posix_memalign((void**)&array, alignment, dim1 * sizeof(*array)) != 0) 
		{
			printf("Memory Alignment Error\n");
			return 1;
		}
	}
	else if(way_to_align == 1)
	{
		//Allocated size must be a power of 2
		array = aligned_alloc(alignment, dim1 * sizeof(*array));
		if(array == NULL) 
		{
			printf("Memory Alignment Error\n");
			return 1;
		}
	}
	else if(way_to_align == 2)
	{
		array = (int (*)[dim2][dim3][dim4][dim5])malloc(dim1 * sizeof(*array));
		if(array == NULL) 
		{
			printf("Memory Alignment Error\n");
			return 1;
		}
	}
	else if(way_to_align == 3)
	{
		//The array is initialized with 0. However, it is slower than malloc.
		array = (int (*)[dim2][dim3][dim4][dim5])calloc(dim1, sizeof(*array));
		if(array == NULL) 
		{
			printf("Memory Alignment Error\n");
			return 1;
		}
	}
	int count = 0;
	begin_time(1);
	for(int repeat = 0; repeat < 10; repeat++){
		for (int i = 0; i < dim1; i++) {
			for (int j = 0; j < dim2; j++) {
				for (int k = 0; k < dim3; k++) {
					for (int l = 0; l < dim4; l++) {
						for(int m = 0; m < dim5; m++){
							array[i][j][k][l][m] = m;
							count += array[i][j][k][l][m];
						}
					}
				}
			}
		}
	}
	end_time(1);
	printf("\ncount:%d", count);

    free(array);

    return 0;
}

