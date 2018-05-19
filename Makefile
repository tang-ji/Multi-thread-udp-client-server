CC = g++

client.o: client.c
	$(CC) -c client.c

server.o: server.c
	$(CC) -c server.c


all: server client

clean: 
	rm -f client server
	rm -f *.o
	
