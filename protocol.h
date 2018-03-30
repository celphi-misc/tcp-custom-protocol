// This file includes the custom protocol used in the experiment
#ifndef __CELPHI_PROTOCOL_H
#define __CELPHI_PROTOCOL_H
// Note that the protocol transmits in big endian
// Below is the definition of our protocol: 
// Protocol format
// HEX  || BE EF |             |       |
// byte || -- -- | -- -- -- -- | -- -- | --...
// desc || Magic | Body Length | Type  | Body

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef enum {
    REQ_TIME        = 0x0001,
    REQ_HOSTNAME    = 0x0002,
    REQ_SOCK_DESC   = 0x0003,
    REQ_SOCK_ALL    = 0x0004,
    REQ_CLIENT_IP   = 0x0005,
    REQ_CLIENT_PORT = 0x0006,
    REQ_SEND_MSG    = 0x0007,
    RPL_TIME        = 0x0101,
    RPL_HOSTNAME    = 0x0102,
    RPL_SOCK_DESC   = 0x0103,
    RPL_SOCK_ALL    = 0x0104,
    RPL_CLIENT_IP   = 0x0105,
    RPL_CLIENT_PORT = 0x0106,
    RPL_SEND_MSG    = 0x0107
} MessageType;

typedef struct {
    int valid;
    MessageType type;
    // Human readable string as the content
    char *content;
} Message;

#define PROTOCOL_HEADER_LEN 8

// This function is for test use
void print_array_in_hex(unsigned char *array);

// To test if the input is of our protocol
int is_custom_protocol(const unsigned char *buf);
// Get length of the body
int get_body_length(const unsigned char *buf);
// Get message type
MessageType get_msg_type(const unsigned char *buf);

//==== Protocol message generating functions ====
// The return values of the functions are the number of bytes per message

// Write time request message to dest
int request_time_msg(unsigned char *dest);
// Write time reply message to dest
int reply_time_msg(unsigned char *dest);
// Message to time
// The return value is the Unix time in int
int msg2time(const unsigned char *src);

// Write hostname request message to dest
int request_hostname_msg(unsigned char *dest);
// Write hostname reply message to dest
int reply_hostname_msg(unsigned char *dest, const unsigned char *src);
// Message to hostname
int msg2hostname(unsigned char *dest, const unsigned char *src);

Message interpret_raw_msg(const unsigned char *src);

#endif