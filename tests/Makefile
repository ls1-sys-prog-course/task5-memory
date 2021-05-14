PYTHON3 ?= python3
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

PROGS=alloc_free_simple alloc_realloc_free_simple calloc_free_simple alloc_free_medium alloc_realloc_free_medium calloc_free_medium coalescing threadtest

check: $(addprefix ${ROOT_DIR}/,$(PROGS))
	$(PYTHON3) ${ROOT_DIR}/basic_func.py
	$(PYTHON3) ${ROOT_DIR}/reuse_freed.py
	$(PYTHON3) ${ROOT_DIR}/merge_blocks.py
	$(PYTHON3) ${ROOT_DIR}/threadtest.py

${ROOT_DIR}/alloc_free_simple: ${ROOT_DIR}/alloc_free_simple.c
	gcc -Og -g -o $@ $<

${ROOT_DIR}/alloc_realloc_free_simple: ${ROOT_DIR}/alloc_realloc_free_simple.c
	gcc -Og -g -o $@ $<

${ROOT_DIR}/calloc_free_simple: ${ROOT_DIR}/calloc_free_simple.c
	gcc -Og -g -o $@ $<

${ROOT_DIR}/alloc_free_medium: ${ROOT_DIR}/alloc_free_medium.c
	gcc -Og -g -o $@ $<

${ROOT_DIR}/alloc_realloc_free_medium: ${ROOT_DIR}/alloc_realloc_free_medium.c
	gcc -Og -g -o $@ $<

${ROOT_DIR}/calloc_free_medium: ${ROOT_DIR}/calloc_free_medium.c
	gcc -Og -g -o $@ $<

${ROOT_DIR}/coalescing: ${ROOT_DIR}/coalescing.c
	gcc -Og -g -o $@ $<

${ROOT_DIR}/threadtest: ${ROOT_DIR}/threadtest.c
	gcc -Og -g -o $@ $< -lpthread
