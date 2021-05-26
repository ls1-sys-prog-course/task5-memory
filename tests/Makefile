PYTHON3 ?= python3
CFLAGS += -O2
CPPFLAGS += $(CFLAGS)
CPPFLAGS += -w
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

PROGS=alloc_free_simple alloc_realloc_free_simple calloc_free_simple\
	  overlap_check_1 overlap_check_2 overlap_check_3\
	  alloc_free_medium alloc_realloc_free_medium calloc_free_medium\
	  coalescing coalescing_multiple\
	  threadtest larson

check: $(addprefix ${ROOT_DIR}/,$(PROGS))
	$(PYTHON3) ${ROOT_DIR}/basic_func.py
	$(PYTHON3) ${ROOT_DIR}/reuse_freed.py
	$(PYTHON3) ${ROOT_DIR}/merge_blocks.py
	$(PYTHON3) ${ROOT_DIR}/threadtest.py

${ROOT_DIR}/alloc_free_simple: ${ROOT_DIR}/alloc_free_simple.c
	$(CC) $(CFLAGS) -Og -g -o $@ $<

${ROOT_DIR}/alloc_realloc_free_simple: ${ROOT_DIR}/alloc_realloc_free_simple.c
	$(CC) $(CFLAGS) -Og -g -o $@ $<

${ROOT_DIR}/calloc_free_simple: ${ROOT_DIR}/calloc_free_simple.c
	$(CC) $(CFLAGS) -Og -g -o $@ $<

${ROOT_DIR}/overlap_check_1: ${ROOT_DIR}/overlap_check_1.c
	$(CC) $(CFLAGS) -Og -g -o $@ $<

${ROOT_DIR}/overlap_check_2: ${ROOT_DIR}/overlap_check_2.c
	$(CC) $(CFLAGS) -Og -g -o $@ $<

${ROOT_DIR}/overlap_check_3: ${ROOT_DIR}/overlap_check_3.c
	$(CC) $(CFLAGS) -Og -g -o $@ $<

${ROOT_DIR}/alloc_free_medium: ${ROOT_DIR}/alloc_free_medium.c
	$(CC) $(CFLAGS) -Og -g -o $@ $<

${ROOT_DIR}/alloc_realloc_free_medium: ${ROOT_DIR}/alloc_realloc_free_medium.c
	$(CC) $(CFLAGS) -Og -g -o $@ $<

${ROOT_DIR}/calloc_free_medium: ${ROOT_DIR}/calloc_free_medium.c
	$(CC) $(CFLAGS) -Og -g -o $@ $<

${ROOT_DIR}/coalescing: ${ROOT_DIR}/coalescing.c
	$(CC) $(CFLAGS) -Og -g -o $@ $<

${ROOT_DIR}/coalescing_multiple: ${ROOT_DIR}/coalescing_multiple.c
	$(CC) $(CFLAGS) -Og -g -o $@ $<

${ROOT_DIR}/threadtest: ${ROOT_DIR}/threadtest.c
	$(CC) $(CFLAGS) -Og -g -o $@ $< -lpthread

${ROOT_DIR}/larson: ${ROOT_DIR}/larson.cpp
	$(CXX) $(CPPFLAGS) -Og -g -o $@ $< -lpthread

clean:
	rm -rf $(PROGS)
