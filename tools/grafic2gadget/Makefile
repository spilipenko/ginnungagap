# Copyright (C) 2010, 2011, Steffen Knollmann
# Released under the terms of the GNU General Public License version 3.
# This file is part of `ginnungagap'.

include ../../Makefile.config

.PHONY: all clean tests tests-clean dist-clean

progName = grafic2gadget

sources = main.c \
          $(progName).c

include ../../Makefile.rules

all:
	$(MAKE) $(progName)

clean:
	rm -f $(progName) $(sources:.c=.o)

tests:
	@echo "No tests yet"

tests-clean:
	@echo "No tests yet to clean"

dist-clean:
	$(MAKE) clean
	rm -f $(sources:.c=.d)

install: $(progName)
	mv -f $(progName) $(BINDIR)/

$(progName): $(sources:.c=.o) \
	                 ../../src/libcosmo/libcosmo.a \
	                 ../../src/libutil/libutil.a
	$(CC) $(LDFLAGS) $(CFLAGS) \
	  -o $(progName) $(sources:.c=.o) \
	                 ../../src/libcosmo/libcosmo.a \
	                 ../../src/libutil/libutil.a \
	                 $(LIBS)

-include $(sources:.c=.d)

../../src/libcosmo/libcosmo.a:
	$(MAKE) -C ../../src/libcosmo

../../src/libutil/libutil.a:
	$(MAKE) -C ../../src/libutil
