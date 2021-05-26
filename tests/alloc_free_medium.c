#include "helper.h"

#define ALLOC_OPS 10000
#define ALLOC_SIZE 1000
#define MAX_THRES 0.75

/*
        Test case:
        Already freed blocks should be reused to pass this test
    Simple policy is sufficient
*/

int main() {
  char *ptr[ALLOC_OPS];

  struct rlimit limit;
  limit.rlim_cur = ALLOC_OPS * ALLOC_SIZE * MAX_THRES;
  limit.rlim_max = ALLOC_OPS * ALLOC_SIZE * MAX_THRES;

  if (setrlimit(RLIMIT_DATA, &limit) != 0) {
    fprintf(stderr, "setrlimit() failed with errno=%d\n", errno);
    exit(1);
  }

  for (int i = 0; i < ALLOC_OPS; i++) {
    ptr[i] = malloc(ALLOC_SIZE);
    if (ptr[i] == NULL) {
      fprintf(stderr, "Fatal: failed to allocate %u bytes.\n", ALLOC_SIZE);
      exit(1);
    }
    if (!IS_SIZE_ALIGNED(ptr[i])) {
      fprintf(stderr, "Returned memory address is not aligned\n");
      exit(1);
    }
    /* access the allocated memory */
    memset(ptr[i], i, ALLOC_SIZE);
    *(ptr[i]) = 's';                  // start
    *(ptr[i] + ALLOC_SIZE - 1) = 'e'; // end
    if (i % 2 == 0) {
      if (!((*ptr[i] == 's' && *(ptr[i] + ALLOC_SIZE - 1) == 'e'))) {
        fprintf(stderr, "Memory content different than the expected\n");
        exit(1);
      }
      free(ptr[i]);
    }
  }

  for (int i = 1; i < ALLOC_OPS; i += 2) {
    if (!((*ptr[i] == 's' && *(ptr[i] + ALLOC_SIZE - 1) == 'e'))) {
      fprintf(stderr, "Memory content different than the expected\n");
      exit(1);
    }
    free(ptr[i]);
  }

  return 0;
}