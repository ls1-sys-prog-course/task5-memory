# Memory management
During this week you are requested to implement your own set of (the well-known) memory allocation functions. \
It should be comprised of:
1. `void *malloc(size_t size)`
2. `void free(void *ptr)`
3. `void *calloc(size_t nitems, size_t size)`
4. `void *realloc(void *ptr, size_t size)`

## Deliverables
You should implement your versions of the aforementioned functions and generate a shared library called `libmymalloc.so`.
This library will be preloaded in existing applications which perform memory management operations.

## Requirements
In order to successfully complete this task, your allocator should comply with the following requirements:
1. The payload pointers returned by malloc should be 8-bytes aligned.
2. The allocator should perform memory allocations under a memory allocation policy of your choice. In order to pass all the tests, 
the implemented allocation strategy should be able to reuse the already freed blocks and to split or merge the free blocks, if necessary.
Limiting heap size waste is crucial in some test cases.
3. Your allocator should be thread-safe and able to scale as the number of application threads that perform memory operations increase.

## Test Setup
There are **four** different test sets that should be executed successfully to earn all the points for this exercise.

### Basic functionality tests
The first set of test cases simply performs a set of memory management operations (malloc, free, realloc, calloc) 
and tries to access and set the allocated memory range. Alignment checks are also performed. There is no constraint in the heap size.

### Efficient heap memory management tests - Reuse free blocks
The second set of test cases behaves similarly with the first one but the heap size is **limited**. 
In order to pass this test set, your memory allocator should be able to reuse the already freed memory blocks appropriately. 
The heap extension limits are set according to each test memory allocation patterns and sizes.

### More efficient heap memory management tests - Split & merge free blocks
The third set of test cases highlights that simply reusing the existing free blocks is not sufficient in some cases. 
You should include in your memory management logic the operation of merging free memory blocks to accommodate the requested memory operations 
and avoid continuous heap extensions if memory allocations can be served in the already acquired heap space.

### Multithreaded allocator tests
In the fourth and last test set, your allocator behaviour will be tested in a multithreaded environment. 
In the first test, each thread repeatedly allocates and then deallocates a certain number of objects.
The second one simulates a server. Each thread allocates and deallocates objects and then transfers some objects (randomly selected) to other threads to be freed [[6]](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.45.1947&rep=rep1&type=pdf).
In both scenarios, your allocator should be able to perform the requested memory operations successfully in a **thread-safe** manner.
Along with that, and since you are now experienced with the concurrent programming from the previous task, the performance of your allocator should scale accordingly when increasing the number of the worker threads.

## Going further
1. If you want to have a quick overview of how the widely used **gibc's malloc** is actually working behind the scenes: https://sourceware.org/glibc/wiki/MallocInternals
2. **Hoard** is another high performant, scalable and memory efficient allocator. More information about its memory management and layout can be found 
in the respective paper: https://people.cs.umass.edu/~emery/pubs/berger-asplos2000.pdf

## General information
1. Edit Makefile to include the build command used for your language
2. For building run 
   ```console
   $ make all
   ```
3. For running the tests run:
   ```console
   $ make check
   ```
4. [gdb - Debugging tip](http://truthbk.github.io/gdb-ld_preload-and-libc/)
5. For Rust, since std::sync::RwLock requires memory allocation to be functional, you are allowed to use parking_lot (https://github.com/Amanieu/parking_lot)

## References:
1. [Valgrind](https://valgrind.org/)
2. [AddressSanitizer: A Fast Address Sanity Checker](https://www.usenix.org/system/files/conference/atc12/atc12-final39.pdf)
3. [Hoard: A Scalable Memory Allocator for Multithreaded Applications](https://people.cs.umass.edu/~emery/pubs/berger-asplos2000.pdf)
4. [MemorySanitizer: fast detector of C uninitialized memory use in C++](https://static.googleusercontent.com/media/research.google.com/en//pubs/archive/43308.pdf)
5. [ThreadSanitizer – data race detection in practice](https://static.googleusercontent.com/media/research.google.com/el//pubs/archive/35604.pdf)
6. [Memory Allocation for Long-Running Server Applications](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.45.1947&rep=rep1&type=pdf)