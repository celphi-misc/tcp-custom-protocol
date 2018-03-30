CC = gcc
OPTIONS = -c
SERVER_RECEIPE = protocol.o server_util.o server.o

server: OPTIONS += -Wall
server: $(SERVER_RECEIPE)
	$(CC) -o server $(SERVER_RECEIPE)
test: protocol.o test.o
	$(CC) -o test protocol.o test.o
client: protocol.o client_test.o
	$(CC) -o client protocol.o client_test.o
all: server

server.o: server.c protocol.c

.PHONY: clean
clean:
	rm -f *.o server client test