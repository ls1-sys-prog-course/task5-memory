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
  void *ptr[ALLOC_OPS];
  int size;
  const char testblock[ALLOC_SIZE] = {0};

  struct rlimit limit;
  limit.rlim_cur = ALLOC_OPS * ALLOC_SIZE * MAX_THRES;
  limit.rlim_max = ALLOC_OPS * ALLOC_SIZE * MAX_THRES;

  if (setrlimit(RLIMIT_DATA, &limit) != 0) {
    fprintf(stderr, "setrlimit() failed with errno=%d\n", errno);
    exit(1);
  }

  for (int i = 0; i < ALLOC_OPS; i++) {
    ptr[i] = calloc(ALLOC_SIZE, sizeof(char));
    if (ptr[i] == NULL) {
      fprintf(stderr, "Fatal: failed to allocate %u bytes.\n", ALLOC_SIZE);
      exit(1);
    }
    if (!IS_SIZE_ALIGNED(ptr[i])) {
      fprintf(stderr, "Returned memory address is not aligned\n");
      exit(1);
    }
    /* check if allocated memory is zeroed */
    if (memcmp(ptr[i], testblock, ALLOC_SIZE) != 0) {
      fprintf(stderr, "Fatal: allocated memory range is not zeroed.\n");
      exit(1);
    }

    if (i % 2 == 0)
      free(ptr[i]);
  }

  for (int i = 1; i < ALLOC_OPS; i += 2) {
    free(ptr[i]);
  }

  return 0;
}