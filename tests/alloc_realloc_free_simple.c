#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

/*
	Test case:
	Correct implementation of realloc is sufficient
	Pass even without block reuse
*/

#define MAX_ALLOC_SIZE 4096
#define MAX_REALLOC_SIZE 4096
#define ALLOC_OPS 10000

int main() {	
	void* ptr[ALLOC_OPS];
	int size;
	
	time_t t;
  	srand((unsigned) time(&t));
	
	for (int i = 0; i < ALLOC_OPS; i++) {
		size = rand() % MAX_ALLOC_SIZE + 1;
		ptr[i] = malloc(size);
		if (ptr[i] == NULL) {
			fprintf(stderr, "Fatal: failed to allocate %u bytes.\n", size);
			exit(-1);
		}
		/* access the allocated memory */
		memset(ptr[i], i, size);
	}

	for (int i = 0; i < ALLOC_OPS; i++) {
		size = rand() % MAX_ALLOC_SIZE + 1;
		ptr[i] = realloc(ptr[i], size);
		if (ptr[i] == NULL) {
			fprintf(stderr, "Fatal: failed to reallocate to %u bytes.\n", size);
			exit(-1);
		}
		/* access the reallocated memory */
		memset(ptr[i], i+1, size);
	}

	for (int i = 0; i < ALLOC_OPS; i++) {
		free(ptr[i]);
	}
	return 0;
}

