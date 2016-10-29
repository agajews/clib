CC=gcc
CFLAGS=-std=c99
SDIR=../lib
ODIR=../obj
HDIR=../libh
INC=-I$(HDIR)
_OBJS = list.o listi.o numer.o str.o tree.o keyval.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

p4: p4.c $(OBJS)
	$(CC) p4.c $(OBJS) -o p4 $(INC) $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

.PHONY: run
run: p4
	./p4

.PHONY: debug
debug: p4
	valgrind --track-origins=yes --leak-check=full ./p4

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o

.PHONY: gloat
gloat:
	wc -l $(SDIR)/* $(HDIR)/* p4.c
