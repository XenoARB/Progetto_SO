CC=gcc
CCOPTS=--std=gnu99 -g -Wall

BINS=shell

OBJS=mmapped_file.o fs_format.o fs_debug.o

HEADERS=mmapped_file.h fs_types.h fs_format.h fs_debug.h

%.o: %.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@ $<

.PHONY: clean all

all: $(BINS)

shell: main.c $(OBJS)
	$(CC) $(CCOPTS) -o $@ $^

clean:
	rm -rf *.o *~ $(BINS)