#
# Shop Sys : a shopping system
#

# Change these variables to point to the appropriate installation directories
INSTALLDIR:=$(HOME)/prg
LIBDIR:=$(INSTALLDIR)/lib
INCDIR:=$(INSTALLDIR)/include

# My code builds without warnings--ALWAYS
CWARNINGS:=	-Wall -Wextra -pedantic \
	-Wmissing-prototypes -Wstrict-prototypes \
	-Wmissing-declarations -Wredundant-decls -Wnested-externs -Wshadow \
	-Wpointer-arith -Wcast-align \
	-Wuninitialized -Wmaybe-uninitialized \
	-Winline -Wno-long-long -Wwrite-strings \
	-Wno-discarded-qualifiers #-Wconversion

CFLAGS:= $(CWARNINGS) --std=c11 -I$(INCDIR) -L$(LIBDIR)

ALLFILES:=customer.c owner.c services.c ShopSys.c global.h
OBJECTS:=services.o owner.o customer.o ShopSys.o
LIBS:=-ldata -linput

CLEANFILES=*.out *.o ShopSys

.PHONEY: all

all: CFLAGS += -O2
all: ShopSys

debug: CFLAGS += -DDEBUG -g
debug: ShopSys


ShopSys: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

################################## UTILITIES ###################################

.PHONEY: clean todolist
clean:
	rm -f $(CLEANFILES)

todolist:
	-@for file in $(ALLFILES:Makefile=); do fgrep -H -e TODO -e FIXME $$file; done; true

