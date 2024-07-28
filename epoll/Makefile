
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic

all: build

build: main.c log.c
	$(CC) $(CFLAGS) main.c log.c -o eserv

clean:
	rm -rf eserv