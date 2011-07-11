CPP=g++
CFLAGS=-Wall -O2 -g
PKG_CONFIG=`pkg-config gtkmm-3.0 --cflags`

_OBJS = elf.o executable.o function.o gui.o instruction.o section.o

SRCDIR = src
OBJS = $(patsubst %,$(SRCDIR)/%,$(_OBJS))

all : edis

%.o : %.cc %.h
	$(CPP) -c -o $@ $< $(CFLAGS) $(PKG_CONFIG)

src/main.o : $(SRCDIR)/main.cc
	$(CPP) -c -o $(SRCDIR)/main.o $(SRCDIR)/main.cc $(CFLAGS) $(PKG_CONFIG)

edis : src/main.o $(OBJS)
	$(CPP) -o edis src/main.o -ldisasm $(OBJS) $(CFLAGS) `pkg-config gtkmm-3.0 --cflags --libs`

clean :
	rm -f src/*.o
