all: lib-ult.c
	gcc -c lib-ult.c -lpthread

exec: lib-ult.c
	gcc -o lib-ult.c -lpthread

clean:
	rm *.o