CC=gcc
objects=main.o logger.o client_handler.o
headers=logger.h client_handler.h client_thread_args.h mutex.h

server: $(objects)
	$(CC) -o server $(objects) -g -lpthread -std=gnu99

main.o: main.c $(headers)
	$(CC) -c main.c

logger.o: logger.c
	$(CC) -c logger.c

client_handler.o: client_handler.c
	$(CC) -c client_handler.c
