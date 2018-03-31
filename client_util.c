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
    struct sockaddr_in socket_addr;
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = inet_addr(ip_addr);
    socket_addr.sin_port = htons(port);
    // Connecting to the remote server
    if(connect(socket_desc, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) < 0)
    {
#ifdef CLIENT_OUTPUT
        perror("Connection failed. Error");
#endif
        return FAILED_CONNECTING;
    }
#ifdef CLIENT_OUTPUT
    puts("Connected");
#endif
    // Here to send the client info to the server
    unsigned char message[MESSAGE_LENGTH];
    int message_length
        = client_info_msg_to_send(message, (unsigned char*)hostname, &socket_addr);
    int send_exit_code = send(socket_desc, (struct sockaddr*)&socket_addr, message_length, 0);
    if(send_exit_code < 0)
    {
#ifdef CLIENT_OUTPUT
        puts("Send failed.");
#endif
        return FAILED_SENDING;
    }
    return socket_desc;
}

// This the routine waiting for the remote server's reply
// This function should run on a separate thread
void receive_reply(const int socket_desc, const int output_desc)
{
    char recv_buffer[MESSAGE_LENGTH];
    // Message buffer contains the human readable string of the received message
    // Which is defined by the protocol
    char mesg_buffer[MESSAGE_LENGTH];
    while(recv(socket_desc, recv_buffer, MESSAGE_LENGTH, 0) > 0)
    {
        // The function below is provided by protocol.h
        // which can convert the message to human readable string
        int msg_type = interpret_raw_msg(mesg_buffer, recv_buffer);
        // If you want some more features, the returned MessageType value
        // can be useful.
        // ==== To be implemented (optional) ====

        // Here to write the string to the output device
        write(output_desc, mesg_buffer, strlen(mesg_buffer) + 1);
    }
}

// Client request functions
int request_hostname(int socket_desc)
{
    char buffer[MESSAGE_LENGTH];
    int mesg_length = request_hostname_msg(buffer);
    return send(socket_desc, buffer, mesg_length, 0);
}

int request_time(int socket_desc)
{
    char buffer[MESSAGE_LENGTH];
    int mesg_length = request_time_msg(buffer);
    return send(socket_desc, buffer, mesg_length, 0);
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
