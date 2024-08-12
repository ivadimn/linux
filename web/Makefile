
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic

all: build

build: server.c log.c
	$(CC) $(CFLAGS) server.c log.c -o eserv -luring

clean:
	rm -rf eserv