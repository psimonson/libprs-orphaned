CC?=gcc
CFLAGS?=-std=c89 -Wall -Wextra -Wno-unused-parameter -g
CFLAGS+=-I../include -D_XOPEN_SOURCE=600
LDFLAGS=

BUILD_MAJOR=0
BUILD_MINOR=1
BUILD_VERSION="$(BUILD_MAJOR).$(BUILD_MINOR)"

SRCDIR=$(shell pwd)
DESTDIR=
PREFIX=usr/local

SRCS=$(SRCDIR)/file.c
OBJS=$(SRCS:.c=.c.o)
TRGT=libcfile

.PHONY: all clean dist dist-clean install uninstall
all: $(SRCDIR)/$(TRGT)

$(SRCDIR)/%.c.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(SRCDIR)/file.c.o: $(SRCDIR)/file.h

$(SRCDIR)/$(TRGT): $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@.so $(OBJS) $(LDFLAGS)
	$(AR) rcs $@.a $(OBJS)

clean:
	rm -f $(SRCDIR)/*~ $(SRCDIR)/*.log \
	$(OBJS) \
	$(SRCDIR)/$(TRGT).a $(SRCDIR)/$(TRGT).so

dist: dist-clean
#	cd $(SRCDIR)/.. && tar --exclude=.git -cvjpf \
#	Sed-$$(date +'%Y-%m-%d').tar.bz2 $$(basename $(SRCDIR))

help:
	grep '^[^[[:space:]]*:' Makefile

dist-clean: clean
#	rm -f $(SRCDIR)/config.h

install:
	mkdir -p $(DESTDIR)/$(PREFIX)/lib
	install $(SRCDIR)/$(TRGT) $(DESTDIR)/$(PREFIX)/lib
	ldconfig

uninstall:
	rm -f $(DESTDIR)/$(PREFIX)/lib/$(TRGT)

version:
	@echo $(BUILD_VERSION)
