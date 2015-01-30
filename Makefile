CC = gcc
CFLAGS = -c -ansi -Wall
LINKS = -lreadline

all: rsi

rsi: rsi.o processing.o process_list.o
	$(CC) rsi.o processing.o process_list.o $(LINKS) -o rsi

rsi.o: rsi.c
	$(CC) $(CFLAGS) rsi.c

processing.o: processing.c
	$(CC) $(CFLAGS) processing.c

process_list.o: process_list.c
	$(CC) $(CFLAGS) process_list.c

clean:
	rm -rf *.o rsi
