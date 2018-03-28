#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Constants
const unsigned HOSTNAME_LENGTH = 2048;
const unsigned MESSAGE_LENGTH = 4096;

// Global variables of the server
char hostname[HOSTNAME_LENGTH];

// Exit code definations
#define SUCCEED_EXITCODE        0
#define FAILED_HOSTNAME         -1
#define FAILED_SOCKET_CREATION  -2
#define FAILED_BINDING          -3

// Initialize the server routine
int init(void)
{
    // Initialize the server's hostname
    if(!gethostname(hostname, HOSTNAME_LENGTH))
        return FAILED_HOSTNAME;
    return SUCCEED_EXITCODE;
}

// Main routine of the server
// This function takes one parameter with the port number given
// The return value is either an error code or a socket descriptor
int new_socket(uint16_t port)
{
    // Create socket
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc == -1)
        return FAILED_SOCKET_CREATION;
    puts("Socket created.");

    // Prepare the socketaddr_in structure
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // Binding
    if(bind(socket_desc, (struct sockaddr*)&server, sizeof(server)))
        return FAILED_BINDING;
    puts("Binded.");

    // Listen
    listen(socket_desc, 3);
    return socket_desc;
}

int main(int argc, char **argv)
{
    // Initialize the server program
    if(init() != SUCCEED_EXITCODE) return FAILED_HOSTNAME;
    
    int socket_desc = new_socket(8888);
    

    return 0;
}