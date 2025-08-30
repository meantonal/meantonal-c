CC = cc
CFLAGS = -Wall -Wextra -Iinclude -Itests

SRCS := $(wildcard src/*.c)
TESTS := $(wildcard tests/test_*.c)

build:
	./amalgamate.sh

test: $(TESTS)
	$(CC) $(CFLAGS) -o run_tests $(TESTS) $(SRCS)
	./run_tests; retval=$$?; rm -f run_tests; exit $$retval
