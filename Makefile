CC=gcc
CFLAGS= -Wall
LIBS=-lm -ljack

build: main.c
	$(CC) $(CFLAGS) main.c -o $@ $(LIBS)
