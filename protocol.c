#include "protocol.h"

// Protocol format
// HEX  || BE EF |             |       |
// byte || -- -- | -- -- -- -- | -- -- | --...
// desc || Magic | Body Length | Type  | Body

// Magic number of the protocol
const uint16_t MAGIC = 0xBEEF;

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
int msg2hostname(unsigned char *dest, unsigned char *src)
{
    strncpy(dest, src + PROTOCOL_HEADER_LEN, get_body_length(src));
    dest[get_body_length(src)] = 0;
    return strlen(dest);
}

// Convert the time reply message to Unix time
int msg2time(unsigned char *src)
{
    return ntohl(*(uint32_t*)(src + PROTOCOL_HEADER_LEN));
}