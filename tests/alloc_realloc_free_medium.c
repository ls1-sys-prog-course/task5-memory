#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

/*
	Test case:
	Already freed blocks should be reused to pass this test
    Simple policy is sufficient
*/

#define ALLOC_SIZE_SMALL 1000
#define ALLOC_SIZE_BIG 2000
#define ALLOC_OPS 10000
#define MAX_THRES 0.85 // 0.75 is maximum actual data, provide 0.1 extra for allocator's metadata

void *allocate_memory (size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
		fprintf(stderr, "Fatal: failed to allocate %lu bytes.\n", size);
		exit(-1);
	}
    memset(ptr, size, size); 
}

int main() {	
	void* ptr[ALLOC_OPS];
	int size;
	
    struct rlimit limit;
	limit.rlim_cur = ALLOC_OPS * ALLOC_SIZE_BIG * MAX_THRES;
	limit.rlim_max = ALLOC_OPS * ALLOC_SIZE_BIG * MAX_THRES;
     
	if (setrlimit(RLIMIT_DATA, &limit) != 0) {
		fprintf(stderr, "setrlimit() failed with errno=%d\n", errno);
		exit(-1);
	}
	
    time_t t;
  	srand((unsigned) time(&t));

    /* 
        allocate big & small blocks 
    */ 
	for (int i = 0; i < ALLOC_OPS; i++) {
        if (i % 2 == 0)
		    ptr[i] = allocate_memory(ALLOC_SIZE_BIG);
        else 
            ptr[i] = allocate_memory(ALLOC_SIZE_SMALL);
    }

    /*
        free big blocks
    */
    for (int i = 0; i < ALLOC_OPS; i+=2) {
        free(ptr[i]);
    }

    /*
        reallocate small ones to use the created big blocks
    */
    for (int i = 1; i < ALLOC_OPS; i+=2) {
        ptr[i] = realloc(ptr[i], ALLOC_SIZE_BIG);
        if (ptr[i] == NULL) {
			fprintf(stderr, "Fatal: failed to reallocate to %u bytes.\n", size);
			exit(-1);
		}
		memset(ptr[i], i+1, size);
    }

	for (int i = 1; i < ALLOC_OPS; i+=2) {
		free(ptr[i]);
	}
	return 0;
}