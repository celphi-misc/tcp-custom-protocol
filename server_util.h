// This file serves as the interface of the server utility methods
#ifndef __CELPHI_SERVER_UTIL_H
#define __CELPHI_SERVER_UTIL_H

#include "commons.h"
#include "client_list.h"

typedef struct package package;
struct package {
    int client_sock;
    char *recv_buf;
};

// Methods

// Create a new socket on the referred port number
// The return value is either an error code (negative)
// or a socket descriptor.
int new_socket(uint16_t port);
// Block to wait for accepting client connection
// The argument is the socket descriptor
// For successful acceptance, the method returns SUCCEED_EXITCODE
int accept_client(int server_sock_desc, int *client_sock_desc);
// And this is the server thread routine
void service(int client_sock_desc);

#endif