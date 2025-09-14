CC = cc
CFLAGS = -Wall -Wextra -Iinclude -Itests
LDFLAGS = -lm

SRCS := $(wildcard src/*.c)
TESTS := $(wildcard tests/test_*.c)

build:
	./amalgamate.sh

test: $(TESTS)
	$(CC) $(CFLAGS) -o run_tests $(TESTS) $(SRCS) $(LDFLAGS)
	./run_tests; retval=$$?; rm -f run_tests; exit $$retval
