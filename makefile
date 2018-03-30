CC = gcc
OPTIONS = -c

server: OPTIONS += -Wall
server: protocol.o server.o
	$(CC) -o server protocol.o server.o
test: protocol.o test.o
	$(CC) -o test protocol.o test.o
client: protocol.o client_test.o
	$(CC) -o client protocol.o client_test.o
all: server

server.o: server.c protocol.c

.PHONY: clean
clean:
	rm -f *.o server