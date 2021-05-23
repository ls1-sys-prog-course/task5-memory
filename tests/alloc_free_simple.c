#include "helper.h"

#define MAX_ALLOC_SIZE 4096
#define MIN_ALLOC_SIZE 2
#define ALLOC_OPS 10000

/*
        Test case:
        Correct implementation of malloc with sbrk() is sufficient
        Pass even without block reuse
*/
int main() {
  char *ptr[ALLOC_OPS];
  int size[ALLOC_OPS];

  time_t t;
  srand((unsigned)time(&t));

  for (int i = 0; i < ALLOC_OPS; i++) {
    size[i] = rand() % MAX_ALLOC_SIZE + MIN_ALLOC_SIZE;
    ptr[i] = malloc(size[i]);
    if (ptr[i] == NULL) {
      fprintf(stderr, "Fatal: failed to allocate %u bytes.\n", size[i]);
      exit(-1);
    }
    assert(IS_SIZE_ALIGNED(ptr[i]));
    /* access the allocated memory */
    memset(ptr[i], i, size[i]);
    *(ptr[i]) = 's';               // start
    *(ptr[i] + size[i] - 1) = 'e'; // end
  }

  for (int i = 0; i < ALLOC_OPS; i++) {
    assert(*ptr[i] == 's' && *(ptr[i] + size[i] - 1) == 'e');
    free(ptr[i]);
  }
  return 0;
}
