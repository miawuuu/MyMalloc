OBJS	= mymalloc.o memgrind.o
SOURCE	= mymalloc.c memgrind.c
HEADER	= mymalloc.h
OUT	= memgrind.c
CC	 = gcc
CFLAGS	 = -g -Wall -fsanitize=address,undefined -std=c99

all: memgrind memtest

memgrind: memgrind.o mymalloc.o 
	$(CC)  $(CFLAGS) $^ -o $@

memtest: memtest.o mymalloc.o
	$(CC) $(CFLAGS) $^ -o $@

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c $< -o $@ -lm

check: check.o mymalloc.o
	$(CC) $(CFLAGS) $^ -o $@

error: error.o mymalloc.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

memgrind.o memtest.o mymalloc.o check.o error.o: mymalloc.h

clean:
	rm -f *.o memtest memgrind
