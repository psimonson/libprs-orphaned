CFLAGS?=-std=c89 -Wall -Wextra -Werror -Wno-unused-parameter
CFLAGS+=-I../src
LDFLAGS=-static -L../src -lprs

.PHONY: all clean
all: test test2

%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: test.c.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test2: test2.c.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *~ *.log *.c.o test test2
