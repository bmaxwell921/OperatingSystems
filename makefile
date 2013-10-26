all: lib-ult.c
	gcc -c lib-ult.c -lpthread

clean:
	rm *.o