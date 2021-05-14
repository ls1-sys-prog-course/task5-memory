#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * extend_heap_size: 
 */

void *extend_heap_size(size_t size) {
  void* current_base = sbrk(0);
  void *extended = sbrk(size);
  assert(extended != (void*) -1); // Note: not thread-safe
  
  /*
   * Modify this function according to your needs
   */

  return extended;
}


void *malloc(size_t size) {

  /*
   * Insert malloc implementation here
   */

  return NULL;
}	

void *calloc(size_t nitems, size_t nsize) {
  
  /*
   * Insert calloc implementation here
   */

  return NULL;
}

void free(void *ptr) {
  
  /*
   * Insert free implementation here
   */

  return;
}

void *realloc(void *ptr, size_t size) {
  
  /*
   * Insert realloc implementation here
   */

  return NULL;
}