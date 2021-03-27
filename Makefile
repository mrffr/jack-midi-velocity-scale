CC=gcc
CFLAGS= -Wall
LIBS=-lm -ljack

midi-vel-scale: main.c
	$(CC) $(CFLAGS) main.c -o $@ $(LIBS)
