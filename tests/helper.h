#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>

#define ALIGNMENT 8

#define IS_SIZE_ALIGNED(ptr) ((((uintptr_t)ptr) & ((ALIGNMENT)-1)) == 0)