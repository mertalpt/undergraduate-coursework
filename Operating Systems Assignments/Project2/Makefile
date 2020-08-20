all: libhash.a test integer-count

libhash.a: hash.c
	gcc -Wall -c hash.c
	ar -cvq libhash.a hash.o
	ranlib libhash.a

integer-count: libhash.a integer-count.c
	gcc -Wall -o integer-count integer-count.c -L. -lhash -lpthread

test: libhash.a test.c
	gcc -Wall -o test test.c -L. -lhash -lpthread

clean:
	rm -rf *.o *.a integer-count test