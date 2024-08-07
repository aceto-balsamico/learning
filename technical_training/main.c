// main.c
#include <stdio.h>
#include <stdlib.h>
#include "libmaze.h"

int main() {
	srand(100);
    struct Node *start = create_maze();
    struct Node *current = start;

    printf("Following the smallest value path...\n");
    
        printf("Current Node Value: %d, %p, %p\n", current->value, current->next1, current->next2);
		current = current->next1;
		printf("Current Node Value: %d, %p, %p\n", current->value, current->next1, current->next2);
		// current = current->next2;
		// printf("Current Node Value: %d, %p, %p\n", current->value, current->next1, current->next2);
		// current = current->next2;
		// printf("Current Node Value: %d, %p, %p\n", current->value, current->next1, current->next2);
		// current = current->next1;
		// printf("Current Node Value: %d, %p, %p\n", current->value, current->next1, current->next2);
		// current = current->next1;
		// printf("Current Node Value: %d, %p, %p\n", current->value, current->next1, current->next2);
		// current = current->next2;
		// printf("Current Node Value: %d, %p, %p\n", current->value, current->next1, current->next2);
		// break;

        if (current->next1 == NULL && current->next2 == NULL) {
            // 終端ノードに到達
            if (current->value == 999) {
                printf("Message: You've reached the end of the maze!\n");
            } else {
                printf("Reached a dead end.\n");
            }
            // break;
        }
		

      

    // free_maze(start);
    return 0;
}