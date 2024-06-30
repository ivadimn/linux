
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic 

all: parse

parse: main.c proc_status.c str_dict.c common.c
	$(CC) $(CFLAGS) main.c proc_status.c str_dict.c common.c -o ps

clean:
	rm -rf ps