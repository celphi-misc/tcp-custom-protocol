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
    for(int i = 0; i < len + PROTOCOL_HEADER_LEN; i++)
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

// Convert the hostname reply message to real hostname
int msg2hostname(unsigned char *dest, const unsigned char *src)
{
    strncpy(dest, src + PROTOCOL_HEADER_LEN, get_body_length(src));
    dest[get_body_length(src)] = 0;
    return strlen(dest);
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
    return PROTOCOL_HEADER_LEN + strlen((char*)hostname) + 6;
}

// Interpreting client info at the server side
// The return value is the length of the hostname
int msg2client_info(unsigned char *dest_hostname, struct sockaddr_in *dest_sockaddr, const unsigned char *src)
{
    int body_length = get_body_length(dest_hostname);

    // Copy IP address and port
    dest_sockaddr->sin_family       = AF_INET;
    dest_sockaddr->sin_addr.s_addr  = ntohl(*(uint32_t*)(src + 8));
    dest_sockaddr->sin_port         = ntohs(*(uint16_t*)(src + 12));
    
    // Copy hostname
    strncpy((char*)dest_hostname, (char*)(src + 14), body_length - 6);
    dest_hostname[body_length - 6] = 0;
    return body_length - 6;
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
            strcpy(dest, ctime(&msg_time));
            return RPL_TIME;
        }
        case RPL_HOSTNAME:
            msg2hostname(dest, src);
            return RPL_HOSTNAME;
        case RPL_SOCK_DESC:     break;
        case RPL_SOCK_ALL:      break;
        case RPL_CLIENT_IP:     break;
        case RPL_CLIENT_PORT:   break;
        case RPL_SEND_MSG:      break;
        case RPL_BOUND:         break;
    }
    return UNKNOWN_TYPE;
}