#include "protocol.h"

// Protocol format
// HEX  || BE EF |             |       |
// byte || -- -- | -- -- -- -- | -- -- | --...
// desc || Magic | Body Length | Type  | Body

// Magic number of the protocol
const uint16_t MAGIC = 0xBEEF;

// Test use
void print_array_in_hex(unsigned char *array)
{
    int len = ntohl(*(int*)(array + 2));
    for(int i = 0; i < len + PROTOCOL_HEADER_LEN && i < 64; i++)
    {
        printf("%2x ", array[i]);
        if(i%16 == 15) puts("");
    }
    puts("");
}

// To test if the input is of our protocol
int is_custom_protocol(const unsigned char *buf)
{
    if(*(uint16_t*)(buf) == htons(MAGIC)) return 1;
    return 0;
}

// Get length of the body
int get_body_length(const unsigned char *buf)
{
    return ntohl(*(uint32_t*)(buf + 2));
}

// Get message type
MessageType get_msg_type(const unsigned char *buf)
{
    return ntohs(*(uint16_t*)(buf + 6));
}

// Write time request message to dest
// The return value is the length of the whole message
int request_time_msg(unsigned char *dest)
{
    // Write magic
    *(uint16_t*)(dest)       = htons(MAGIC);
    // Write body length
    *(uint32_t*)(dest + 2)   = 0;
    // Write type
    *(uint16_t*)(dest + 6)   = htons(REQ_TIME);
    return PROTOCOL_HEADER_LEN;
}

// Write hostname request message to dest
int request_hostname_msg(unsigned char *dest)
{
    // Write magic
    *(uint16_t*)(dest)       = htons(MAGIC);
    // Write body length
    *(uint32_t*)(dest + 2)   = 0;
    // Write type
    *(uint16_t*)(dest + 6)   = htons(REQ_HOSTNAME);
    return PROTOCOL_HEADER_LEN;
}

// Write request message to dest that lists all descriptor
int request_listing_clients_msg(unsigned char *dest)
{
    // Write magic
    *(uint16_t*)(dest)       = htons(MAGIC);
    // Write body length
    *(uint32_t*)(dest + 2)   = 0;
    // Write type
    *(uint16_t*)(dest + 6)   = htons(REQ_SOCK_ALL);
    return PROTOCOL_HEADER_LEN;
}

// Write the client communication request message
int request_comm_msg(unsigned char *dest, int to_desc, const char *content)
{
    // Write magic
    *(uint16_t*)(dest)      = htons(MAGIC);
    // Write body length
    *(uint32_t*)(dest + 2)  = htonl(4 + strlen(content));
    // Write type
    *(uint16_t*)(dest + 6)  = htons(REQ_SEND_MSG);
    // Write to_desc
    *(uint32_t*)(dest + PROTOCOL_HEADER_LEN) = htonl(to_desc);
    // Write message
    strncpy((char*)dest, content, strlen(content));
    return PROTOCOL_HEADER_LEN + 4 + strlen(content);
}

// Write the message to the corresponding client
int reply_comm_msg(unsigned char *dest, int from_desc, const char *content)
{
    // Write magic
    *(uint16_t*)(dest)      = htons(MAGIC);
    // Write body length
    *(uint32_t*)(dest + 2)  = htonl(4 + strlen(content));
    // Write type
    *(uint16_t*)(dest + 6)  = htons(RPL_SEND_MSG);
    // Write from_desc
    *(uint32_t*)(dest + PROTOCOL_HEADER_LEN) = htonl(from_desc);
    // Write messge
    strncpy((char*)dest + PROTOCOL_HEADER_LEN + 4, content, strlen(content));
    return PROTOCOL_HEADER_LEN + 4 + strlen(content);
}

// Message to real content
// The return value is the from/to descriptor
int msg2content(unsigned char *dest, const unsigned char *src)
{
    int content_length = ntohl(*(uint32_t*)(src + 2)) - 4;
    int to = ntohl(*(uint32_t*)(src + PROTOCOL_HEADER_LEN));
    strncpy((char*)dest, (char*)src + PROTOCOL_HEADER_LEN + 4, content_length);
    dest[content_length] = 0;
    return to;
}

// Write time reply message to dest
int reply_time_msg(unsigned char *dest)
{
    int now = time(NULL);
    // Write magic
    *(uint16_t*)(dest)       = htons(MAGIC);
    // Write body length
    *(uint32_t*)(dest + 2)   = htonl(sizeof(now));
    // Write type
    *(uint16_t*)(dest + 6)   = htons(RPL_TIME);
    // Write body
    *(int32_t*)(dest + PROTOCOL_HEADER_LEN)    = htonl(now);
    return PROTOCOL_HEADER_LEN + sizeof(now);
}

// Write hostname reply message to dest
int reply_hostname_msg(unsigned char *dest, const unsigned char *src)
{
    // Write magic
    *(uint16_t*)(dest)       = htons(MAGIC);
    // Write body length
    *(uint32_t*)(dest + 2)   = htonl(strlen((char*)src));
    // Write type
    *(uint16_t*)(dest + 6)   = htons(RPL_HOSTNAME);
    // Write body
    strcpy((char*)(dest + PROTOCOL_HEADER_LEN), (char*)src);
    return PROTOCOL_HEADER_LEN + strlen((char*)src);
}

int reply_listing_clients_msg(unsigned char *dest, const int n,
        const int *desc_list, const struct sockaddr_in *socket_addr_list)
{
    // Write magic
    *(uint16_t*)(dest)      = htons(MAGIC);
    // Write body length
    *(uint32_t*)(dest + 2)  = htonl(n * 10);
    // Write type
    *(uint16_t*)(dest + 6)  = htons(RPL_SOCK_ALL);
    char *body_pointer = (char*)dest + PROTOCOL_HEADER_LEN;
    // Write body
    for(int i = 0; i < n; i++)
    {
        *(uint32_t*)(body_pointer) = htonl(desc_list[i]);
        *(uint32_t*)(body_pointer + 4) = htonl(socket_addr_list[i].sin_addr.s_addr);
        *(uint16_t*)(body_pointer + 8) = htons(socket_addr_list[i].sin_port);
        body_pointer += 10;
    }
    return PROTOCOL_HEADER_LEN + n * 10;
}

int msg2client_list(int *desc_list,
    struct sockaddr_in *socket_addr_list, const unsigned char *src)
{
    int body_length = ntohl(*(uint32_t*)(src + 2));
    int n = body_length/10;
    for(int i = 0; i < n; i++)
    {
        desc_list[i] = ntohl(*(uint32_t*)(src + PROTOCOL_HEADER_LEN + i * 10));
        socket_addr_list[i].sin_family = AF_INET;
        socket_addr_list[i].sin_addr.s_addr = ntohl(*(uint32_t*)(src + PROTOCOL_HEADER_LEN + i * 10 + 4));
        socket_addr_list[i].sin_port = ntohs(*(uint16_t*)(src + PROTOCOL_HEADER_LEN + i * 10 + 8));
    }
    return n;
}

// Convert the hostname reply message to real hostname
int msg2hostname(unsigned char *dest, const unsigned char *src)
{
    strncpy((char*)dest, (const char*)src + PROTOCOL_HEADER_LEN, get_body_length(src));
    dest[get_body_length(src)] = 0;
    return strlen((char*)dest);
}

// Convert the time reply message to Unix time
int msg2time(const unsigned char *src)
{
    return ntohl(*(uint32_t*)(src + PROTOCOL_HEADER_LEN));
}

// Client sending its hostname and IP address
// The return value is the length of the whole message
int client_info_msg_to_send(unsigned char *dest, const unsigned char *hostname, const struct sockaddr_in *socket_addr)
{
    // Write magic
    *(uint16_t*)(dest)       = htons(MAGIC);
    // Write body length
    *(uint32_t*)(dest + 2)   = htonl(strlen((char*)hostname) + 6);
    // Write type
    *(uint16_t*)(dest + 6)   = htons(CLIENT_INFO);
    // Write IP address (uint32_t, 4B)
    *(uint32_t*)(dest + 8)   = htonl(socket_addr->sin_addr.s_addr);
    // Write port       (uint16_t, 2B)
    *(uint16_t*)(dest + 12)  = htons(socket_addr->sin_port);
    // Write hostname
    strcpy((char*)(dest + 14), (char*)hostname);
#ifdef PROTOCOL_TEST
    printf("Data size: %lu\n", PROTOCOL_HEADER_LEN + strlen((char*)hostname) + 6);
    print_array_in_hex(dest);
#endif
    return PROTOCOL_HEADER_LEN + strlen((char*)hostname) + 6;
}

// Interpreting client info at the server side
// The return value is the length of the hostname
int msg2client_info(unsigned char *dest_hostname, struct sockaddr_in *dest_sockaddr, const unsigned char *src)
{
    int body_length = get_body_length(src);

    // Copy IP address and port
    dest_sockaddr->sin_family       = AF_INET;
    dest_sockaddr->sin_addr.s_addr  = ntohl(*(uint32_t*)(src + 8));
    dest_sockaddr->sin_port         = ntohs(*(uint16_t*)(src + 12));
    
    // Copy hostname
    strncpy((char*)dest_hostname, (char*)(src + 14), body_length - 6);
    dest_hostname[body_length - 6] = 0;
    return body_length - 6;
}
