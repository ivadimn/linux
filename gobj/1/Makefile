DEBUG=-g
CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic `pkg-config --cflags gtk4` $(DEBUG)
LDFLAGS=`pkg-config --libs gtk4`

BINARY=main

build: main.c demowidget.c
	$(CC) $(CFLAGS) main.c demowidget.c -o main $(LDFLAGS)

clean:
	rm -f $(BINARY)
