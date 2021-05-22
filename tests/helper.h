#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

#define ALIGNMENT 8

#define IS_SIZE_ALIGNED(ptr) ((((uintptr_t)ptr) & ((ALIGNMENT) - 1)) == 0)