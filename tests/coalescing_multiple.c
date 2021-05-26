#include "helper.h"

/*
        Test case: Coalescing Multiple - Merge adjacent blocks is necessary
*/

#define ALLOC_SIZE 500
#define ALLOC_OPS 10000
#define NEW_ALLOC_OPS ALLOC_OPS / 4
#define MAX_ADDED_SIZE 3 * ALLOC_SIZE

#define MAX_THRES 1.1 // 1 for the allocated memory + 10% for the block metadata

int main() {
  char *ptr[ALLOC_OPS];
  char *new_ptr[NEW_ALLOC_OPS];
  size_t new_size[NEW_ALLOC_OPS];

  struct rlimit limit;
  limit.rlim_cur = ALLOC_OPS * ALLOC_SIZE * MAX_THRES;
  limit.rlim_max = ALLOC_OPS * ALLOC_SIZE * MAX_THRES;

  if (setrlimit(RLIMIT_DATA, &limit) != 0) {
    fprintf(stderr, "setrlimit() failed with errno=%d\n", errno);
    exit(1);
  }

  time_t t;
  srand((unsigned)time(&t));

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
  }

  /* free 4 consecutive ptrs */
  for (int i = 0; i < ALLOC_OPS; i++) {
    if (i % 5 > 0) {
      if (!((*ptr[i] == 's' && *(ptr[i] + ALLOC_SIZE - 1) == 'e'))) {
        fprintf(stderr, "Memory content different than the expected\n");
        exit(1);
      }
      free(ptr[i]);
    }
  }

  for (int i = 0; i < NEW_ALLOC_OPS; i++) {
    new_size[i] = rand() % MAX_ADDED_SIZE + ALLOC_SIZE;
    new_ptr[i] = malloc(new_size[i]);
    if (new_ptr[i] == NULL) {
      fprintf(stderr, "Fatal: failed to allocate %lu bytes.\n", new_size[i]);
      exit(1);
    }
    if (!IS_SIZE_ALIGNED(new_ptr[i])) {
      fprintf(stderr, "Returned memory address is not aligned\n");
      exit(1);
    }
    /* access the allocated memory */
    memset(new_ptr[i], i, new_size[i]);
    *(new_ptr[i]) = 's';                   // start
    *(new_ptr[i] + new_size[i] - 1) = 'e'; // end
  }

  for (int i = 0; i < ALLOC_OPS; i++) {
    if (i % 5 == 0) {
      if (!((*ptr[i] == 's' && *(ptr[i] + ALLOC_SIZE - 1) == 'e'))) {
        fprintf(stderr, "Memory content different than the expected\n");
        exit(1);
      }
      free(ptr[i]);
    }
  }

  for (int i = 0; i < NEW_ALLOC_OPS; i += 2) {
    if (!((*new_ptr[i] == 's' && *(new_ptr[i] + new_size[i] - 1) == 'e'))) {
      fprintf(stderr, "Memory content different than the expected\n");
      exit(1);
    }
    free(new_ptr[i]);
  }
  return 0;
}