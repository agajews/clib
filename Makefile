CC=gcc
CFLAGS=-std=c99
CLIB?=.
SDIR=$(CLIB)/lib
ODIR=$(CLIB)/obj
HDIR=$(CLIB)/libh
INC=-I$(HDIR)
_OBJS = list.o listi.o stack.o iter.o \
array.o str.o tree.o rbtree.o keyval.o \
numer.o file.o tuple.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

clib: $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.c $(HDIR)/%.h
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

.PHONY: cleanlib
cleanlib:
	rm -f $(ODIR)/*.o
