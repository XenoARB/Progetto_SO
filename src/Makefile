CC=gcc
CCOPTS=--std=gnu99 -g -Wall

BINS=shell

OBJS=mmapped_file.o

HEADERS=mmapped_file.h

%.o: %.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@ $<

.PHONY: clean all

all: $(BINS)

shell: main.c $(OBJS)
	$(CC) $(CCOPTS) -o $@ $^

clean:
	rm -rf *.o *~ $(BINS)