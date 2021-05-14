#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <time.h>

#define MAX_ALLOC_SIZE 4096
#define ALLOC_OPS 10000

/*
	Test case:
	Correct implementation of malloc with sbrk() is sufficient
	Pass even without block reuse
*/
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
		free(ptr[i]);
	}
	return 0;
}

