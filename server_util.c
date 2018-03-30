#include "server_util.h"

// Global variables of the server
char hostname[HOSTNAME_LENGTH];
// Init flag
int intialized;

// Initialize the server (initializing the hostname is enough)
int init(void)
{
    if(!intialized) intialized = 1;
    if(gethostname(hostname, HOSTNAME_LENGTH))
        return FAILED_HOSTNAME;
    return SUCCEED_EXITCODE;
}

// Create a new socket on the referred port number
// The return value is either an error code (negative)
// or a socket descriptor.
int new_socket(uint16_t port)
{
    int init_status;
    if(!intialized) init_status = init();
    if(init_status != SUCCEED_EXITCODE) return init_status;
    // Create socket
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc == -1)
    {
#ifdef TEST_OUTPUT
        perror("Socket creation failed.");
#endif
        return FAILED_SOCKET_CREATION;
    }
#ifdef TEST_OUTPUT
    puts("Socket created.");
#endif
    // sockaddr_in strcuture
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // Binding
    if(bind(socket_desc, (struct sockaddr*)&server, sizeof(server)))
    {
#ifdef TEST_OUTPUT
        perror("Binding failed.");
#endif
        return FAILED_BINDING;
    }
#ifdef TEST_OUTPUT
    puts("Binded");
#endif

    // Listening
    listen(socket_desc, 3);
    return socket_desc;
}

// This should be the main service of the server
// The argument is the socket descriptor
int service(int sock_desc)
{
    int client_sock;
    struct sockaddr_in client;
    int addlen = sizeof(struct sockaddr_in);

    // Accept a connection
    if(client_sock = accept(socket_desc, (struct sockaddr*)&client, (socklen_t*)&addrlen))
    {
#ifdef TEST_OUTPUT
        puts("Connection accepted");
#endif
        // This is client message buffer
        char client_message[MESSAGE_LENGTH];
        int read_size;
        while((read_size = recv(client_sock, client_message, 2000, 0)) > 0)
        {
            // TODO
        }
    }
    return 0;
}