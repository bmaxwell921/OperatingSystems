CC = gcc
CFLAGS = -g -pedantic

all: MainTest Given lib-ult.o

MainTest: MainTest.o lib-ult.o
	$(CC) $(CFLAGS) -o MainTest MainTest.o lib-ult.o -lpthread

MainTest.o: MainTest.c lib-ult.h
	$(CC) $(CFLAGS) -c MainTest.c

Given: Given.o lib-ult.o
	$(CC) $(CFLAGS) -o Given Given.o lib-ult.o -lpthread

Given.o: Given.c lib-ult.h
	$(CC) $(CFLAGS) -c Given.c		

lib-ult.o: lib-ult.c
	$(CC) -c -pedantic lib-ult.c -lpthread

clean:
	rm *.o