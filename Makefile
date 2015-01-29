CC = gcc
CFLAGS = -c -ansi -Wall
LINKS = -lreadline

all: rsi

rsi: rsi.o processing.o
	$(CC) rsi.o processing.o $(LINKS) -o rsi

rsi.o: rsi.c
	$(CC) $(CFLAGS) rsi.c

processing.o: processing.c
	$(CC) $(CFLAGS) processing.c

clean:
	rm -rf *.o rsi
