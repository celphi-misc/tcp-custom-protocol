CC = gcc
OPTIONS = -c

server: OPTIONS += -Wall
server: protocol.o server.o
	$(CC) -o server protocol.o server.o
all: server

server.o: server.c server.h protocol.c protocol.h
