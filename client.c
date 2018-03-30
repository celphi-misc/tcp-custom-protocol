#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Constants
#define CLIENT_NAME_LENGTH  2048
#define PORT 8888

// Global variables of the client
char clientname[CLIENT_NAME_LENGTH];
char hostname[CLIENT_NAME_LENGTH];

// Exit code definations
#define SUCCEED_EXITCODE                0
#define FAILED_CLIENT_NAME_UNCONFIG     -1
#define FAILED_SOCKET_CREATION          -2
#define FAILED_SOCKET_CONNECTION        -3
#define FAILED_SERVER_NO_RESPONSE       -4

/** Main routine of the client
 * int* socket_desc : a pointer of socket descriptor's id
 * int port         : default port number to the server
 * return an exit code
 **/        
int new_socket(int* socket_desc, int port)
{
    if((*socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        puts("ERROR::SOCKET_GENERATE::SOCKET");
        socket_desc = NULL;
        return FAILED_SOCKET_CREATION;
    }
    puts("Socket generated.");

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;
    
    // connect 
    if (connect(*socket_desc, (struct sockaddr*)&server, sizeof(server)) == -1){
        puts("ERROR::CONNECT::SOCKET");
        return FAILED_SOCKET_CONNECTION;
    }
    puts("Connected.");
    return SUCCEED_EXITCODE;
}

int init() 
{
    if(!gethostname(clientname, CLIENT_NAME_LENGTH))
        return FAILED_CLIENT_NAME_UNCONFIG;
    strcpy(hostname, clientname);
    return SUCCEED_EXITCODE;
}

int main() 
{
    int exit_code;
    // Initialize client name
    if((exit_code = init()) != SUCCEED_EXITCODE) return exit_code;
    
    int socket_desc;
    if((exit_code = new_socket(&socket_desc, PORT)) != SUCCEED_EXITCODE) return exit_code;

    // TODO: CLI-design & recv implementation
    return 0;
}