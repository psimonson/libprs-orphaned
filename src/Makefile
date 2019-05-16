CC?=gcc
CFLAGS?=-std=c89 -Wall -Wextra -Wno-unused-parameter -g
BITS?=$(shell getconf LONG_BIT)
CFLAGS+=-D_XOPEN_SOURCE=600 -m$(BITS)
LDFLAGS=

BUILD_MAJOR=0
BUILD_MINOR=1
BUILD_VERSION="$(BUILD_MAJOR).$(BUILD_MINOR)"

SRCDIR=$(shell pwd)
DESTDIR?=
PREFIX?=usr/local
TRGTDIR=prslib

HDRS=$(SRCDIR)/file.h $(SRCDIR)/clogger.h
SRCS=$(SRCDIR)/file.c $(SRCDIR)/clogger.c
OBJS=$(SRCS:.c=.c.o)
TRGT=libprs

.PHONY: all clean dist dist-clean install uninstall
all: $(SRCDIR)/$(TRGT)

$(SRCDIR)/%.c.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(SRCDIR)/file.c.o: $(SRCDIR)/file.h
$(SRCDIR)/clogger.c.o: $(SRCDIR)/file.h $(SRCDIR)/clogger.h

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
	mkdir -p $(DESTDIR)/$(PREFIX)/lib$(BITS)/$(TRGTDIR)
	install $(SRCDIR)/$(TRGT).a \
		$(SRCDIR)/$(TRGT).so \
		$(DESTDIR)/$(PREFIX)/lib$(BITS)/$(TRGTDIR)
	mkdir -p $(DESTDIR)/$(PREFIX)/include/$(TRGTDIR)
	install $(HDRS) $(DESTDIR)/$(PREFIX)/include/$(TRGTDIR)
	cp prs.pc $(DESTDIR)/$(PREFIX)/lib$(BITS)/pkgconfig
	ldconfig
else ifeq ($(BITS),32)
	mkdir -p $(DESTDIR)/$(PREFIX)/lib/$(TRGTDIR)
	install $(SRCDIR)/$(TRGT).a \
		$(SRCDIR)/$(TRGT).so \
		$(DESTDIR)/$(PREFIX)/lib/$(TRGTDIR)
	mkdir -p $(DESTDIR)/$(PREFIX)/include/$(TRGTDIR)
	install $(HDRS) $(DESTDIR)/$(PREFIX)/include/$(TRGTDIR)
	sed 's/\/usr\/lib64/\/usr\/lib' prs.pc > \
		$(DESTDIR)/$(PREFIX)/lib/pkgconfig/prs.pc
	ldconfig
else
	echo "Cannot install for $(BITS) bit architecture." || die
endif

uninstall:
ifeq ($(BITS),64)
	rm -f $(DESTDIR)/$(PREFIX)/lib$(BITS)/$(TRGTDIR)/$(TRGT).a \
	$(DESTDIR)/$(PREFIX)/lib$(BITS)/$(TRGTDIR)/$(TRGT).so
	rmdir $(DESTDIR)/$(PREFIX)/lib$(BITS)/$(TRGTDIR)
	rm -f $(DESTDIR)/$(PREFIX)/include/$(TRGTDIR)/*.h
	rmdir $(DESTDIR)/$(PREFIX)/include/$(TRGTDIR)
else ifeq ($(BITS),32)
	rm -f $(DESTDIR)/$(PREFIX)/lib/$(TRGTDIR)/$(TRGT).a \
	$(DESTDIR)/$(PREFIX)/lib/$(TRGTDIR)/$(TRGT).so
	rmdir $(DESTDIR)/$(PREFIX)/lib/$(TRGTDIR)
	rm -f $(DESTDIR)/$(PREFIX)/include/$(TRGTDIR)/*.h
	rmdir $(DESTDIR)/$(PREFIX)/include/$(TRGTDIR)
else
	echo "Cannot build for $(BITS) bit architecture." || die
endif

version:
	@echo $(BUILD_VERSION)
