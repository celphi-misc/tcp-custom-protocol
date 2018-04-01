// Definitions of the client and the server that are in common
#ifndef __CELPHI_COMMONS_H
#define __CELPHI_COMMONS_H

#include "protocol.h"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Constants
#define MESSAGE_LENGTH      4096
#define HOSTNAME_LENGTH     64
#define READABLE_IP_LENGTH  17
// The maximum number of clients that the server can handle
#define MAX_CLIENT          64

// Toggle server output
#define SERVER_OUTPUT

// Exit code definations
#define SUCCEED_EXITCODE         0
#define FAILED_HOSTNAME         -1
#define FAILED_SOCKET_CREATION  -2
#define FAILED_BINDING          -3
#define FAILED_CONNECTING       -4
#define FAILED_SENDING          -5
#define MAX_CLIENT_EXCEEDED     -6
#define CLIENT_NOT_FOUND        -7
#define UNKNOWN_PROTOCOL        -8

#endif