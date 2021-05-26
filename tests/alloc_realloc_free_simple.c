#include "helper.h"

/*
        Test case:
        Correct implementation of realloc is sufficient
        Pass even without block reuse
*/

#define MAX_ALLOC_SIZE 4096
#define MAX_REALLOC_SIZE 4096
#define MIN_ALLOC_SIZE 2
#define ALLOC_OPS 10000

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

  size_t old_size;
  for (int i = 0; i < ALLOC_OPS; i++) {
    old_size = size[i];
    size[i] = rand() % MAX_ALLOC_SIZE + MIN_ALLOC_SIZE;
    ptr[i] = realloc(ptr[i], size[i]);
    if (ptr[i] == NULL) {
      fprintf(stderr, "Fatal: failed to reallocate to %u bytes.\n", size[i]);
      exit(-1);
    }
    assert(IS_SIZE_ALIGNED(ptr[i]));
    /* check if the content is copied */
    if (old_size < size[i]) // enlarge
      assert(*ptr[i] == 's' && *(ptr[i] + old_size - 1) == 'e');
    else // shrink
      assert(*ptr[i] == 's');

    /* access the reallocated memory */
    memset(ptr[i], i + 1, size[i]);
    *(ptr[i]) = 's';               // start
    *(ptr[i] + size[i] - 1) = 'e'; // end
  }

  for (int i = 0; i < ALLOC_OPS; i++) {
    assert(*ptr[i] == 's' && *(ptr[i] + size[i] - 1) == 'e');
    free(ptr[i]);
  }
  return 0;
}