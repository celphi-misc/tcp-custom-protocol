#include "client_util.h"

// Global variables of the client
char clientname[CLIENT_NAME_LENGTH];

int flag = false;

// TODO: To be refactored
#if 0
int init() 
{
    if(gethostname(clientname, CLIENT_NAME_LENGTH))
    {
#ifdef TEST_OUTPUT
        puts("ERROR::INIT::CLIENT_NAME");
#endif
        return FAILED_HOSTNAME;
    }
    strcpy(hostname, clientname);
    return SUCCEED_EXITCODE;
}
#endif

int new_socket(int* socket_desc, int port)
{
    if((*socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
#ifdef TEST_OUTPUT
        puts("ERROR::NEW_SOCKET::SOCKET_CREATION");
#endif
        socket_desc = NULL;
        return FAILED_SOCKET_CREATION;
    }
#ifdef TEST_OUTPUT
    puts("Socket generated.");
#endif

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;
    
    // connect 
    if (connect(*socket_desc, (struct sockaddr*)&server, sizeof(server)) == -1)
    {
#ifdef TEST_OUTPUT    
        puts("ERROR::NEW_SOCKET::SOCKET_CONNECTING");
#endif
        return FAILED_CONNECTING;
    }
#ifdef TEST_OUTPUT
    puts("Connected.");
#endif
    flag = true;
    return SUCCEED_EXITCODE;
}

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