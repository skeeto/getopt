.POSIX:
CC     = cc
CFLAGS = -ansi -pedantic -Wall -Wextra -O3 -g3

tests: tests.c getopt.h
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ tests.c $(LDLIBS)

test: check
check: tests
	./tests

clean:
	rm -f tests
