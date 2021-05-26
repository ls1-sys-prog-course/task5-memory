#include "helper.h"

#define MAX_ALLOC_SIZE 4096
#define ALLOC_OPS 10000

/*
        Test case:
        Correct implementation of calloc is sufficient
        Pass even without block reuse
*/

int main() {
  void *ptr[ALLOC_OPS];
  int size;
  const char testblock[MAX_ALLOC_SIZE] = {0};
  time_t t;
  srand((unsigned)time(&t));

  for (int i = 0; i < ALLOC_OPS; i++) {
    size = rand() % MAX_ALLOC_SIZE + 1;
    ptr[i] = calloc(size, sizeof(char));
    if (ptr[i] == NULL) {
      fprintf(stderr, "Fatal: failed to allocate %u bytes.\n", size);
      exit(1);
    }
    if (!IS_SIZE_ALIGNED(ptr[i])) {
      fprintf(stderr, "Returned memory address is not aligned\n");
      exit(1);
    }
    /* check if allocated memory is zeroed */
    if (memcmp(ptr[i], testblock, size) != 0) {
      fprintf(stderr, "Fatal: allocated memory range is not zeroed.\n");
      exit(1);
    }
  }

  for (int i = 0; i < ALLOC_OPS; i++) {
    free(ptr[i]);
  }
  return 0;
}