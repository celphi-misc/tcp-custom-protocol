#ifndef __ZEPHYR_CLIENT_UTIL_H
#define __ZEPHYR_CLIENT_UTIL_H

#include "commons.h"

// Methods

// initialize client name and host name
int init();
// Create a new socket on the referred port number and IP address
// The return value is either an error code (negative)
// or a socket descriptor 
int new_socket(uint16_t port, const char *ip_addr);
// This the routine waiting for the remote server's reply
// This function should run on a separate thread
// The first argument should be the socket file descriptor
// that you have first created
// The second argument of the function is the file descriptor
// for the output file, eg: STDOUT_FILENO
void receive_reply(const int socket_desc, const int output_fd);

// Client request functions
// The return value is the exit code of "send" function
int request_hostname(int socket_desc);
int request_time(int socket_desc);
int request_listing_clients(int socket_desc);
int request_send_message(int socket_desc, int to_desc, const char *src);

#endif
