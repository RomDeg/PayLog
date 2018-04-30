NAME=main
SRCS=main.c
CC=gcc
CFLAGS=-g -Wall
GTKFLAGS=-export-dynamic `pkg-config --cflags --libs gtk+-3.0`
all: main

main: $(SRCS) paylog.o
	$(CC) $(CFLAGS) paylog.o -o $(NAME) $(SRCS) $(GTKFLAGS)
paylog: paylog.c
	$(CC) -std=c99 -c paylog.c
clean:
	/bin/rm -f $(NAME) paylog.o