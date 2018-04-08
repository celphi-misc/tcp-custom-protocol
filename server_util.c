#include "server_util.h"

// Global variables of the server
unsigned char hostname[HOSTNAME_LENGTH];
// Init flag
int intialized;

// Initialize the server (initializing the hostname is enough)
int init(void)
{
    if(!intialized) intialized = 1;
    if(gethostname((char*)hostname, HOSTNAME_LENGTH))
        return FAILED_HOSTNAME;
    return SUCCEED_EXITCODE;
}

// Server reply current time 
int server_action_rpl_time(
    int client_sock, const unsigned char *recv_buffer)
{
    unsigned char mesg_buffer[MESSAGE_LENGTH];
    int mesg_length = reply_time_msg(mesg_buffer);
    // Write to the socket
    write(client_sock, mesg_buffer, mesg_length);

    return SUCCEED_EXITCODE;
}

// Server reply hostname
int server_action_rpl_hostname(
    int client_sock, const unsigned char *recv_buffer)
{
    unsigned char mesg_buffer[MESSAGE_LENGTH];
    int mesg_length = reply_hostname_msg(mesg_buffer, hostname);
    write(client_sock, mesg_buffer, mesg_length);

    return SUCCEED_EXITCODE;
}

// Server reply client list
int server_action_rpl_client_list(
    int client_sock, const unsigned char *recv_buffer)
{
    int desc_list[MAX_CLIENT];
    char** name_list=(char**)malloc(sizeof(char*) * MAX_CLIENT);
    struct sockaddr_in socket_addr_list[MAX_CLIENT];
    unsigned char mesg_buffer[MESSAGE_LENGTH];
    int n = 0;
    for(int i = 0; i < MAX_CLIENT; i++)
    {
        if(client_list[i].desc > 0)
        {
            desc_list[n] = client_list[i].desc;
            name_list[n] = (char*)malloc(sizeof(char*)*MAX_NAME_LENGTH);
            strcpy(name_list[n], (char*)client_list[i].name);
            socket_addr_list[n] = client_list[i].socket_addr;
            n++;
        }
    }
    int mesg_length = reply_listing_clients_msg(mesg_buffer, n, desc_list, socket_addr_list, name_list);
#ifdef PROTOCOL_TEST
    print_array_in_hex(mesg_buffer);
#endif
    write(client_sock, mesg_buffer, mesg_length);

    return SUCCEED_EXITCODE;
}

int server_action_rpl_comm_msg(
    int client_sock, const unsigned char *recv_buffer)
{
    unsigned char content[MESSAGE_LENGTH];
    int to_desc = msg2content(content, recv_buffer);
    unsigned char mesg_buffer[MESSAGE_LENGTH];
    int mesg_length = reply_comm_msg(mesg_buffer, client_sock, (char*)content);
    if(find_socket_addr(to_desc) == NULL)
    {
        printf("Orienting client not found, message sending cancelled.\n");
        unsigned char mesg_back[MESSAGE_LENGTH];
        int mesg_back_length = reply_comm_msg_sender(mesg_back, 0);
        write(client_sock, mesg_back, mesg_back_length);
        return 0;
    }
    else 
    {
        int ret = write(to_desc, mesg_buffer, mesg_length);
        unsigned char mesg_back[MESSAGE_LENGTH];
        int mesg_back_length = reply_comm_msg_sender(mesg_back, ret);
        write(client_sock, mesg_back, mesg_back_length);
        return ret;
    }
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
#ifdef SERVER_OUTPUT
        perror("Socket creation failed");
#endif
        return FAILED_SOCKET_CREATION;
    }
#ifdef SERVER_OUTPUT
    puts("Socket created.");
#endif
    // sockaddr_in strcuture
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // Reuse addr
    int on = 1;
    if(setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))< 0)
    {
#ifdef SERVER_OUTPUT
        perror("Set socket option failed");
#endif
        return(FAILED_SOCKET_CREATION);
    }

    // Binding
    if(bind(socket_desc, (struct sockaddr*)&server, sizeof(server)))
    {
#ifdef SERVER_OUTPUT
        perror("Binding failed");
#endif
        return FAILED_BINDING;
    }
#ifdef SERVER_OUTPUT
    puts("Binded");
#endif

    // Listening
    listen(socket_desc, 3);
    return socket_desc;
}

// The argument is the socket descriptor
int accept_client(int sock_desc, int *client_sock)
{
#ifdef SERVER_OUTPUT
    puts("Waiting for connection.");
#endif
    struct sockaddr_in client;
    int addrlen = sizeof(struct sockaddr_in);

    // Accept a connection
    if((*client_sock = accept(sock_desc, (struct sockaddr*)&client, (socklen_t*)&addrlen)))
    {
#ifdef SERVER_OUTPUT
        puts("Connection accepted");
#endif
        // This is the client message buffer
        unsigned char recv_buffer[MESSAGE_LENGTH];
        int read_size;
        if((read_size = recv(*client_sock, recv_buffer, MESSAGE_LENGTH, 0)) <= 0)
            return SUCCEED_EXITCODE;
#ifdef PROTOCOL_TEST
        printf("Read size: %d\n", read_size);
        print_array_in_hex(recv_buffer);
#endif
        if(!is_custom_protocol((unsigned char*)recv_buffer))
        {
#ifdef SERVER_OUTPUT
            puts("Unknown protocol.");
#endif
            return UNKNOWN_PROTOCOL;
        }
        // Receiving the client info
        // int length = PROTOCOL_HEADER_LEN + get_body_length((unsigned char*)recv_buffer);
        unsigned char hostname[HOSTNAME_LENGTH];
        struct sockaddr_in client_socket_addr;
        msg2client_info(hostname, &client_socket_addr, recv_buffer);
        add_client(*client_sock, (char*)hostname, &client_socket_addr);
        return SUCCEED_EXITCODE;
    }
    return FAILED_CONNECTING;
}

// And this is the server thread routine
void service(int client_sock)
{
    unsigned char recv_buffer[MESSAGE_LENGTH];
    int read_size;
    while((read_size = recv(client_sock, recv_buffer, MESSAGE_LENGTH, 0)) > 0)
    {
#ifdef PROTOCOL_TEST
        printf("Read size: %d\n", read_size);
        print_array_in_hex(recv_buffer);
#endif
        if(!is_custom_protocol(recv_buffer))
        {
#ifdef SERVER_OUTPUT
            puts("Unknown protocol.");
#endif
            return; // Exit thread
        }
        MessageType mesg_type = get_msg_type((unsigned char*)recv_buffer);
#ifdef PROTOCOL_TEST
        printf("%d: \n", mesg_type);
        print_array_in_hex(recv_buffer);
#endif
        switch(mesg_type)
        {
            case REQ_TIME:
                server_action_rpl_time(client_sock, recv_buffer); break;
            case REQ_HOSTNAME: 
                server_action_rpl_hostname(client_sock, recv_buffer); break;
            case REQ_SOCK_ALL:
                server_action_rpl_client_list(client_sock, recv_buffer); break;
            case REQ_SEND_MSG:
            {
                int exit_code = 
                    server_action_rpl_comm_msg(client_sock, recv_buffer);
#ifdef SERVER_OUTPUT
                if(!exit_code)
                {
                    puts("Unable to send message to client.");
                }
#endif
                break;
            }
            case REQ_DISCONNECT:
            {
                int ret;
                if ((ret = remove_client(client_sock)) != CLIENT_NOT_FOUND)
                    printf("client %d disconnected.\n", client_sock);
                else {
                    printf("client %d not found.\n", client_sock);
                }
                return;
            }
            default: break;
        }
    }
}
