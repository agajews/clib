CC=gcc
CFLAGS=-std=c99
CLIB?=.
SDIR=$(CLIB)/lib
ODIR=$(CLIB)/obj
HDIR=$(CLIB)/libh
INC=-I$(HDIR)
_OBJS = list.o listi.o numer.o str.o tree.o keyval.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

clib: $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o
