.POSIX:
CC = cc
CFLAGS = -std=c89 -ansi -Wall -Wextra -O3 -g3
test: test.c getopt.h
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ test.c $(LDLIBS)
clean:
	rm -f test
