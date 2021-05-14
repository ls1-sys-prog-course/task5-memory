#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

/*
	Test case: Coalescing - Merge adjacent blocks is necessary
*/

#define ALLOC_SIZE 1000
#define REALLOC_SIZE 2000
#define ALLOC_OPS 10000
#define MAX_THRES 0.75

int main() {	
	void* ptr[ALLOC_OPS];
	int size;
	
    struct rlimit limit;
	limit.rlim_cur = ALLOC_OPS * REALLOC_SIZE * MAX_THRES;
	limit.rlim_max = ALLOC_OPS * REALLOC_SIZE * MAX_THRES;

	if (setrlimit(RLIMIT_DATA, &limit) != 0) {
		fprintf(stderr, "setrlimit() failed with errno=%d\n", errno);
		exit(-1);
	}
		
	for (int i = 0; i < ALLOC_OPS; i++) {
		ptr[i] = malloc(ALLOC_SIZE);
		if (ptr[i] == NULL) {
			fprintf(stderr, "Fatal: failed to allocate %u bytes.\n", ALLOC_SIZE);
			exit(-1);
		}
		/* access the allocated memory */
		memset(ptr[i], i, size);   
        if (i % 2 == 0) {
            free(ptr[i]);
        }
    }

    for (int i = 1; i < ALLOC_OPS; i+=2) {
        ptr[i] = realloc(ptr[i], REALLOC_SIZE);
        if (ptr[i] == NULL) {
			fprintf(stderr, "Fatal: failed to reallocate to %u bytes.\n", size);
			exit(-1);
		}
		/* access the reallocated memory */
		memset(ptr[i], i+1, size);
    }

	for (int i = 1; i < ALLOC_OPS; i+=2) {
		free(ptr[i]);
	}
	return 0;
}

