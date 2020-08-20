all: app

libralloc.a:  ralloc.c
	gcc -Wall -c ralloc.c
	ar -cvq libralloc.a ralloc.o
	ranlib libralloc.a

app: app.c libralloc.a
	gcc -Wall -o app app.c -L. -lralloc -lpthread

experiment: experiment.c libralloc.a
	gcc -Wall -o experiment experiment.c -L. -lralloc -lpthread

clean: 
	rm -fr *.o *.a *~ a.out app experiment ralloc.o ralloc.a libralloc.a