#include "client_util.h"

// Global variables of the client
char hostname[HOSTNAME_LENGTH];
// Init flag
int initialized;

int init(void)
{
    if(!initialized) initialized = 1;
    if(gethostname(hostname, HOSTNAME_LENGTH))
        return FAILED_HOSTNAME;
    return SUCCEED_EXITCODE;
}

// Create a new socket on the referred port number and IP address
// The return value is either an error code (negative)
// or a socket descriptor 
int new_socket(uint16_t port, const char *ip_addr)
{
    int init_status;
    if(!initialized) init_status = init();
    if(init_status != SUCCEED_EXITCODE) return init_status;
    // Create socket
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc == -1)
    {
#ifdef CLIENT_OUTPUT
        perror("Socket creation failed.");
#endif
        return FAILED_SOCKET_CREATION;
    }
#ifdef CLIENT_OUTPUT
    puts("Socket created.");
#endif
    // sockaddr_in structure
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip_addr);
    server.sin_port = htons(port);
    // Connecting to the remote server
    if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
#ifdef CLIENT_OUTPUT
        perror("Connection failed. Error");
#endif
        return FAILED_CONNECTING;
    }
#ifdef CLIENT_OUTPUT
    puts("Connected");
#endif
    // TODO: Here to send the client info to the server
    return socket_desc;
}

// TODO:

#if 0
void show_welcome() 
{
    printf("+-----------------------------------------------+\n");
    printf("|                                               |\n");
    printf("|  Welcome to Celphi-Zephyr Web Client service  |\n");
    printf("|                                               |\n");
    printf("+-----------------------------------------------+\n");
    printf("|  IP: 127.0.0.1 | Current status: Unconnected  |\n");
    printf("+-----------------------------------------------+\n");
    printf("|  Options:                                     |\n");
    printf("|  1. Connect: \tenter \"connect\" or \"c\".         |\n");
    printf("|  2. Help: \tenter \"help\" or \"h\".            |\n");
    printf("|  3. Exit: \tenter \"exit\" or \"e\".            |\n");
    printf("+-----------------------------------------------+\n");
}
#endif

