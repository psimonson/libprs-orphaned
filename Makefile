CFLAGS=-std=c89 -Wall -Wextra -Werror -Wno-unused-parameter
LDFLAGS=

.PHONY: all clean dist dist-clean install uninstall
all:
	cd src && $(MAKE) all
	cd tests && $(MAKE) all

clean:
	cd src && $(MAKE) clean
	cd tests && $(MAKE) clean
	rm -f *~

dist: dist-clean
	cd $(SRCDIR)/.. && tar --exclude=.git -cvjpf \
	Sed-$$(date +'%Y-%m-%d').tar.bz2 $$(basename $(SRCDIR))

dist-clean: clean
	cd src && $(MAKE) dist-clean
#	cd tests && $(MAKE) dist-clean

install:
	cd src && $(MAKE) install

uninstall:
	cd src && $(MAKE) uninstall
