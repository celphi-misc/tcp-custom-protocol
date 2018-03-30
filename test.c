#include "commons.h"

// Constants
#define HOSTNAME_LENGTH 2048

// Global variables of the server
char hostname[HOSTNAME_LENGTH];

void print_array_in_hex(unsigned char *array)
{
    int len = ntohl(*(int*)(array + 2));
    for(int i = 0; i < len + PROTOCOL_HEADER_LEN; i++)
    {
        printf("%2x ", array[i]);
        if(i%16 == 15) puts("");
    }
    puts("");
}

// Initialize the server routine
int init(void)
{
    // Initialize the server's hostname
    if(gethostname(hostname, HOSTNAME_LENGTH))
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
    unsigned char buf[1024];
    // Initialize the server program
    if(init() != SUCCEED_EXITCODE) return FAILED_HOSTNAME;
    int socket_desc = new_socket(8888);
    
    // Client socket descriptor
    int client_sock;
    struct sockaddr_in client;
    int addrlen = sizeof(struct sockaddr_in);

    if( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&addrlen)) )
    {
        puts("Connection accepted");
        char client_message[MESSAGE_LENGTH];
        int read_size;
        while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
        {
            print_array_in_hex(client_message);
            //Send the message back to client
            int message_len = reply_time_msg(client_message);
            write(client_sock , client_message, message_len);
        }
    }
    reply_time_msg(buf);
    print_array_in_hex(buf);
    return 0;
}