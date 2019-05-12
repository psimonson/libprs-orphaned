CC?=gcc
CFLAGS?=-std=c89 -Wall -Wextra -Wno-unused-parameter -g
BITS?=$(shell getconf LONG_BIT)
CFLAGS+=-I../include -D_XOPEN_SOURCE=600 -m$(BITS)
LDFLAGS=

BUILD_MAJOR=0
BUILD_MINOR=1
BUILD_VERSION="$(BUILD_MAJOR).$(BUILD_MINOR)"

SRCDIR=$(shell pwd)
DESTDIR?=
PREFIX?=usr/local

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
ifeq ($(BITS),64)
	mkdir -p $(DESTDIR)/$(PREFIX)/lib$(BITS)/prs_cfile
	install $(SRCDIR)/$(TRGT).a \
		$(SRCDIR)/$(TRGT).so \
		$(DESTDIR)/$(PREFIX)/lib$(BITS)/prs_cfile
	mkdir -p $(DESTDIR)/$(PREFIX)/include/prs_headers
	install $(SRCDIR)/file.h $(DESTDIR)/$(PREFIX)/include/prs_headers
	ldconfig
else ifeq ($(BITS),32)
	mkdir -p $(DESTDIR)/$(PREFIX)/lib/prs_cfile
	install $(SRCDIR)/$(TRGT).a \
		$(SRCDIR)/$(TRGT).so \
		$(DESTDIR)/$(PREFIX)/lib/prs_cfile
	mkdir -p $(DESTDIR)/$(PREFIX)/include/prs_headers
	install $(SRCDIR)/file.h $(DESTDIR)/$(PREFIX)/include/prs_headers
	ldconfig
else
	echo "Cannot install for $(BITS) bit architecture." || die
endif

uninstall:
ifeq ($(BITS),64)
	rm -f $(DESTDIR)/$(PREFIX)/lib$(BITS)/prs_cfile/$(TRGT).a \
	$(DESTDIR)/$(PREFIX)/lib$(BITS)/prs_cfile/$(TRGT).so
	rmdir $(DESTDIR)/$(PREFIX)/lib$(BITS)/prs_cfile
	rm -f $(DESTDIR)/$(PREFIX)/include/prs_headers/*.h
	rmdir $(DESTDIR)/$(PREFIX)/include/prs_headers
else ifeq ($(BITS),32)
	rm -f $(DESTDIR)/$(PREFIX)/lib/prs_cfile/$(TRGT).a \
	$(DESTDIR)/$(PREFIX)/lib/prs_cfile/$(TRGT).so
	rmdir $(DESTDIR)/$(PREFIX)/lib/prs_cfile
	rm -f $(DESTDIR)/$(PREFIX)/include/prs_headers/*.h
	rmdir $(DESTDIR)/$(PREFIX)/include/prs_headers
else
	echo "Cannot build for $(BITS) bit architecture." || die
endif

version:
	@echo $(BUILD_VERSION)
