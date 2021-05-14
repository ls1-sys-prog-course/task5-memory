#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

#define ALLOC_OPS 10000
#define ALLOC_SIZE 1000
#define MAX_THRES 0.75

/*
	Test case:
	Already freed blocks should be reused to pass this test
    Simple policy is sufficient
*/

int main() {
	void* ptr[ALLOC_OPS];
	int size;

	struct rlimit limit;
	limit.rlim_cur = ALLOC_OPS * ALLOC_SIZE * MAX_THRES;
	limit.rlim_max = ALLOC_OPS * ALLOC_SIZE * MAX_THRES;

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
		memset(ptr[i], i, ALLOC_SIZE);
		if (i % 2 == 0)
			free(ptr[i]);
	}

	for (int i = 1; i < ALLOC_OPS; i+=2) {
		free(ptr[i]);
	}

	return 0;
}

