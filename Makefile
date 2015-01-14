CC = gcc
CFLAGS = -c -ansi -Wall
LINKS = -lreadline

all: rsi

rsi: rsi.o
	$(CC) $(LINKS) rsi.o -o rsi

rsi.o: rsi.c
	$(CC) $(CFLAGS) rsi.c

clean:
	rm -rf *.o rsi
