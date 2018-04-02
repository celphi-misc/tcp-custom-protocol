#include "client_util.h"

// Global variables of the client
unsigned char hostname[HOSTNAME_LENGTH];
// Init flag
int initialized;

int init(void)
{
    if(!initialized) initialized = 1;
    if(gethostname((char*)hostname, HOSTNAME_LENGTH))
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
        perror("Socket creation failed");
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
        perror("Connection failed");
#endif
        return FAILED_CONNECTING;
    }
#ifdef CLIENT_OUTPUT
    puts("Connected");
#endif
    // Here to send the client info to the server
    unsigned char message[MESSAGE_LENGTH];
    int message_length =
        client_info_msg_to_send(message, (unsigned char*)hostname, &socket_addr);
    int send_exit_code
        = send(socket_desc, message, message_length, 0);
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
    unsigned char recv_buffer[MESSAGE_LENGTH];
    // Message buffer contains the human readable string of the received message
    // Which is defined by the protocol
    unsigned char mesg_buffer[MESSAGE_LENGTH];
    int received = recv(socket_desc, recv_buffer, MESSAGE_LENGTH, 0);
#ifdef PROTOCOL_TEST
    print_array_in_hex(recv_buffer);
#endif
    while(received > 0)
    {
        // The function below is provided by protocol.h
        // which can convert the message to human readable string
        // MessageType msg_type = 
            interpret_raw_msg(mesg_buffer, recv_buffer);
        // If you want some more features, the returned MessageType value
        // can be useful.
        // ==== To be implemented (optional) ====

        // Here to write the string to the output device
        write(output_desc, mesg_buffer, strlen((char*)mesg_buffer) + 1);
    }
}

// TODO: This function is to be implemented
MessageType interpret_raw_msg(unsigned char *dest, const unsigned char *src)
{
    MessageType mesg_type = get_msg_type(src);
    if(mesg_type < RPL_TIME || mesg_type >= RPL_BOUND) return UNKNOWN_TYPE;
    switch(mesg_type)
    {
        case RPL_TIME:
        {
            time_t msg_time = msg2time(src);
            strcpy((char*)dest, ctime(&msg_time));
            return RPL_TIME;
        }
        case RPL_HOSTNAME:
            msg2hostname(dest, src);
            return RPL_HOSTNAME;
        case RPL_SOCK_DESC:     return RPL_SOCK_DESC;
        case RPL_SOCK_ALL:
        {
            int desc_list[MAX_CLIENT];
            struct sockaddr_in socket_addr_list[MAX_CLIENT];
            int n = msg2client_list(desc_list, socket_addr_list, src);
            for(int i = 0; i < n; i++)
            {
                sprintf((char*)dest, "%d: %s @ %d\n",
                desc_list[i],
                inet_ntoa(socket_addr_list[i].sin_addr),
                (int) ntohs(socket_addr_list[i].sin_port));
            }
            return RPL_SOCK_ALL;
        }
        case RPL_CLIENT_IP:     return RPL_CLIENT_IP;
        case RPL_CLIENT_PORT:   return RPL_CLIENT_PORT;
        case RPL_SEND_MSG:
        {
            unsigned char content_buffer[MESSAGE_LENGTH];
            int from = msg2content(dest, content_buffer);
            sprintf((char*)dest, "%d: %s\n", from, content_buffer);
            return RPL_SEND_MSG;
        }
        case RPL_BOUND:         return RPL_BOUND;
        default: break;
    }
    return UNKNOWN_TYPE;
}

// Client request functions
int request_hostname(int socket_desc)
{
    unsigned char buffer[MESSAGE_LENGTH];
    int mesg_length = request_hostname_msg(buffer);
    return send(socket_desc, buffer, mesg_length, 0);
}

int request_time(int socket_desc)
{
    unsigned char buffer[MESSAGE_LENGTH];
    int mesg_length = request_time_msg(buffer);
    return send(socket_desc, buffer, mesg_length, 0);
}

int request_listing_clients(int socket_desc)
{
    unsigned char buffer[MESSAGE_LENGTH];
    int mesg_length = request_listing_clients_msg(buffer);
    return send(socket_desc, buffer, mesg_length, 0);
}

int request_send_message(int socket_desc, int to_desc, const char *src)
{
    unsigned char buffer[MESSAGE_LENGTH];
    int mesg_length = request_comm_msg(buffer, to_desc, src);
    return send(socket_desc, buffer, mesg_length, 0);
}