all: bilshell consumer producer

bilshell: bilshell.c
	gcc -Wall -g -o bilshell.o bilshell.c

consumer: consumer.c
	gcc -Wall -g -o consumer.o consumer.c

producer: producer.c
	gcc -Wall -g -o producer.o producer.c

clean:
	rm -rf ./bilshell.o ./consumer.o ./producer.o