# Set you prefererred CFLAGS/compiler compiler here.
# Our github runner provides gcc-10 by default.
CC ?= cc
CFLAGS ?= -g -Wall -O2
CXX ?= c++
CXXFLAGS ?= -g -Wall -O2
CARGO ?= cargo
RUSTFLAGS ?= -g

# this target should build all executables for all tests
all:
	@echo "Please set a concrete build command here"
	false

#all: libmymalloc.so

# C example:
#libmymalloc.so: task5-memory.c
#	$(CC) $(CFLAGS) -shared -fPIC -ldl -o $@ $<

# C++ example:
#task5-memory:
#	$(CXX) $(CXXFLAGS) -o task5-memory task5-memory.cpp

# Rust example:
#all:
#	$(CARGO) build --release

# Usually there is no need to modify this
check: all
	$(MAKE) -C tests check

clean:
	rm -rf *.so *.so.* *.o
