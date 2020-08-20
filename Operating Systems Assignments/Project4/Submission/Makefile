obj-m += hello-1.o
obj-m += print_module.o

all: app.o
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

app.o: app.c
	gcc -Wall -o app.o app.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
