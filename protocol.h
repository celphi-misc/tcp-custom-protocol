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
// #define PROTOCOL_TEST

typedef enum {
    CLIENT_INFO     = 0x0000,
    REQ_TIME        = 0x0001,
    REQ_HOSTNAME    = 0x0002,
    REQ_SOCK_DESC   = 0x0003,
    REQ_SOCK_ALL    = 0x0004,
    REQ_CLIENT_IP   = 0x0005,
    REQ_CLIENT_PORT = 0x0006,
    REQ_SEND_MSG    = 0x0007,
    REQ_BOUND       = 0x0008,   // This is just a boundary, not a flag
    RPL_TIME        = 0x0101,
    RPL_HOSTNAME    = 0x0102,
    RPL_SOCK_DESC   = 0x0103,
    RPL_SOCK_ALL    = 0x0104,
    RPL_CLIENT_IP   = 0x0105,
    RPL_CLIENT_PORT = 0x0106,
    RPL_SEND_MSG    = 0x0107,
    RPL_BOUND       = 0x0108,   // This is just a boundary, not a flag
    UNKNOWN_TYPE    = 0xFFFF
} MessageType;

#define PROTOCOL_HEADER_LEN 8
#define MAX_NAME_LENGTH 64

// This function is for test use
void print_array_in_hex(unsigned char *array);

// To test if the input is of our protocol
int is_custom_protocol(const unsigned char *buf);
// Get length of the body
int get_body_length(const unsigned char *buf);
// Get message type
MessageType get_msg_type(const unsigned char *buf);

// Client sending its hostname and IP address
// This behavior is done as soon as the connection is structured
// The return value is the length of the whole message
// ---- Format ----
// [Header] [uint32_t IP] [uint16_t Port] [char string Hostname]
int client_info_msg_to_send(
    unsigned char *dest, const unsigned char *hostname, const struct sockaddr_in *socket_addr);
// Interpreting client info at the server side
// The return value is the length of the hostname
int msg2client_info(
    unsigned char *dest_hostname, struct sockaddr_in *dest_sockaddr, const unsigned char *src);

// ===============================================
// ==== Protocol message generating functions ====
// ===============================================
// The return values of the functions are the number of bytes per message

// ==== Time ====
// ---- Request format ----
// [Header]
// ---- Reply format ----
// [Header] [int32_t Unix time]
// Write time request message to dest
int request_time_msg(unsigned char *dest);
// Write time reply message to dest
int reply_time_msg(unsigned char *dest);
// Message to time
// The return value is the Unix time in int
int msg2time(const unsigned char *src);

// ==== Hostname ====
// ---- Request format ----
// [Header]
// ----Reply format ----
// [Header] [char string Hostname]
// Write hostname request message to dest
int request_hostname_msg(unsigned char *dest);
// Write hostname reply message to dest
int reply_hostname_msg(unsigned char *dest, const unsigned char *src);
// Message to hostname
int msg2hostname(unsigned char *dest, const unsigned char *src);

// ==== Client list ====
// ---- Request format ----
// [Header]
// ---- Reply format ----
// [Header]
// [uint32_t Socket descriptor 1]
// [uint32_t IP address 1] [uint16_t Port 1]
// [uint32_t Socket descriptor 2]
// [uint32_t IP address 2] [uint16_t Port 2]
//          ...
// Write client list request message to dest
int request_listing_clients_msg(unsigned char *dest);
// Write client list reply message to dest
// The return value is the size of the whole message
int reply_listing_clients_msg(unsigned char *dest, const int n,
    const int *desc_list, const struct sockaddr_in *socket_addr_list, char * const * const name_list);
// Message to client list
// The return value is the number of total clients
int msg2client_list(int *desc_list,
    struct sockaddr_in *socket_addr_list, char **name_list, const unsigned char *src);

// ==== Client communication ====
// ---- Request format ----
// [Header] [uint32_t To client socket number]
// [uint32_t String length] [char string message]
// ---- Reply ----
// Write to the specified client!!
// [Header] [uint32_t From client socket number]
// [uint32_t String length] [char string message]

// Write the client communication request message
int request_comm_msg(unsigned char *dest, int to_desc, const char *content);
// Write the message to the corresponding client
int reply_comm_msg(unsigned char *dest, int from_desc, const char *content);
// Message to real content
// The return value is the from/to descriptor
int msg2content(unsigned char *dest, const unsigned char *src);


// This function converts the receiving message to human reabable string
// And returns the message type
MessageType interpret_raw_msg(unsigned char *dest, const unsigned char *src);

#endif