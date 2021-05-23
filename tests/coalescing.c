#include "helper.h"

/*
        Test case: Coalescing - Merge adjacent blocks is necessary
*/

#define ALLOC_SIZE 1000
#define REALLOC_SIZE 2000
#define ALLOC_OPS 10000
#define MAX_THRES 0.75

int main() {
  char *ptr[ALLOC_OPS];

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
    assert(IS_SIZE_ALIGNED(ptr[i]));
    /* access the allocated memory */
    memset(ptr[i], i, ALLOC_SIZE);
    *(ptr[i]) = 's';                  // start
    *(ptr[i] + ALLOC_SIZE - 1) = 'e'; // end
    if (i % 2 == 0) {
      assert(*ptr[i] == 's' && *(ptr[i] + ALLOC_SIZE - 1) == 'e');
      free(ptr[i]);
    }
  }

  for (int i = 1; i < ALLOC_OPS; i += 2) {
    assert(*ptr[i] == 's' && *(ptr[i] + ALLOC_SIZE - 1) == 'e');
    ptr[i] = realloc(ptr[i], REALLOC_SIZE);
    if (ptr[i] == NULL) {
      fprintf(stderr, "Fatal: failed to reallocate to %u bytes.\n",
              REALLOC_SIZE);
      exit(-1);
    }
    assert(IS_SIZE_ALIGNED(ptr[i]));
    /* access the reallocated memory */
    memset(ptr[i], i + 1, REALLOC_SIZE);
    *(ptr[i]) = 's';                    // start
    *(ptr[i] + REALLOC_SIZE - 1) = 'e'; // end
  }

  for (int i = 1; i < ALLOC_OPS; i += 2) {
    assert(*ptr[i] == 's' && *(ptr[i] + REALLOC_SIZE - 1) == 'e');
    free(ptr[i]);
  }
  return 0;
}