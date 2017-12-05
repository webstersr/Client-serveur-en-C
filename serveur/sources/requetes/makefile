CC=gcc
CFLAGS=-Wall -I../../headers -DTEST
EXEC=test

all: $(EXEC)

test: fonctions.o 
	$(CC) test.c fonctions.o -o $(EXEC) $(CFLAGS)

fonctions.o: 
	$(CC) -c ../fonctions.c $(CFLAGS)

clean:
	rm *.o


