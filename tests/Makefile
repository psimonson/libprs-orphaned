CFLAGS?=-std=c89 -Wall -Wextra -Werror -Wno-unused-parameter
CFLAGS+=-I../include -I../src
LDFLAGS=-static -L../src -lcfile

.PHONY: all clean
all: test

%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: test.c.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *~ *.c.o test
