#include "helper.h"

/*
	Test case:
	Already freed blocks should be reused to pass this test
    Simple policy is sufficient
*/

#define ALLOC_SIZE_SMALL 1000
#define ALLOC_SIZE_BIG 2000
#define ALLOC_OPS 10000
#define MAX_THRES 0.85 // 0.75 is maximum actual data, provide 0.1 extra for allocator's metadata

char *allocate_memory (size_t size) {
    char *ptr = malloc(size);
    if (ptr == NULL) {
		fprintf(stderr, "Fatal: failed to allocate %lu bytes.\n", size);
		exit(-1);
	}
    assert(IS_SIZE_ALIGNED(ptr));
    memset(ptr, size, size); 
    *(ptr) = 's'; //start
	*(ptr + size - 1) = 'e'; //end
    return ptr;
}

int main() {	
	char* ptr[ALLOC_OPS];
	int size[ALLOC_OPS];
	
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
        if (i % 2 == 0) {
		    ptr[i] = allocate_memory(ALLOC_SIZE_BIG);
            size[i] = ALLOC_SIZE_BIG;
        }
        else {
            ptr[i] = allocate_memory(ALLOC_SIZE_SMALL);
            size[i] = ALLOC_SIZE_SMALL;
        }
    }

    /*
        free big blocks
    */
    for (int i = 0; i < ALLOC_OPS; i+=2) {
        assert(*ptr[i] == 's' && *(ptr[i] + size[i] - 1) == 'e');
        free(ptr[i]);
    }

    /*
        reallocate small ones to use the created big blocks
    */
    for (int i = 1; i < ALLOC_OPS; i+=2) {
        assert(*ptr[i] == 's' && *(ptr[i] + size[i] - 1) == 'e');
        ptr[i] = realloc(ptr[i], ALLOC_SIZE_BIG);
        if (ptr[i] == NULL) {
			fprintf(stderr, "Fatal: failed to reallocate to %u bytes.\n", size[i]);
			exit(-1);
		}
        assert(IS_SIZE_ALIGNED(ptr[i]));
        size[i] = ALLOC_SIZE_BIG;
		memset(ptr[i], i+1, size[i]);
        *(ptr[i]) = 's'; //start
	    *(ptr[i] + size[i] - 1) = 'e'; //end
    }

	for (int i = 1; i < ALLOC_OPS; i+=2) {
        assert(*ptr[i] == 's' && *(ptr[i] + size[i] - 1) == 'e');
		free(ptr[i]);
	}
	return 0;
}