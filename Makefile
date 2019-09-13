CC=gcc
CFLAGS=-Wall -Werror

all:
	$(CC) $(CFLAGS) bbcpm.c -o bbcpm

clean:
	rm bbcpm
