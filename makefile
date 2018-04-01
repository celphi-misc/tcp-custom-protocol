# Compiler
CC = gcc
# Compiling options, -Wall for "Warning all"
OPTIONS = -g -c -Wall

# A list of executable files, used for cleaning up
EXECUTABLE = server client test

# Receipes of the server and the client 
SERVER_RECEIPE = protocol.o server_util.o server.o client_list.o
CLIENT_RECEIPE = protocol.o client_util.o client.o

# Linking commands
server: $(SERVER_RECEIPE)
	$(CC) -o server $(SERVER_RECEIPE)
client: $(CLIENT_RECEIPE)
	$(CC) -o client $(CLIENT_RECEIPE)
# Make all
all: server client

%.o: %.c
	$(CC) $(OPTIONS) $<

# Phony target for cleaning up
.PHONY: clean
clean:
	rm -f *.o $(EXECUTABLE)