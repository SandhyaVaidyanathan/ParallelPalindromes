all: main palin

CC = gcc
MOBJS = main.o
POBJS = palin.o
main: $(MOBJS)
	$(CC) -g -o main main.o

palin: $(POBJS)
	$(CC) -g -o palin palin.o

palin.o: shm.h
	$(CC) -g -c palin.c

main.o: shm.h
	$(CC) -g -c main.c
.PHONY: clean
clean:
	 /bin/rm -f core *.o  $(MOBJS) $(POBJS) palin main

