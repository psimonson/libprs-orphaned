CFLAGS?=-std=c89 -Wall -Wextra -Werror -Wno-unused-parameter
CFLAGS+=-I../include -I../src
LDFLAGS=-static -L../src -lcfile

.PHONY: all clean
all: test test2

%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: test.c.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test2: test2.c.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *~ *.c.o test test2
