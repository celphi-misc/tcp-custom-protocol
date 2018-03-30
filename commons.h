// Definitions of the client and the server in common
#ifndef __CELPHI_COMMONS_H
#define __CELPHI_COMMONS_H

#include "protocol.h"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MESSAGE_LENGTH 4096

// Toggle server output
#define SERVER_OUTPUT

// Exit code definations
#define SUCCEED_EXITCODE        0
#define FAILED_HOSTNAME         -1
#define FAILED_SOCKET_CREATION  -2
#define FAILED_BINDING          -3
#define FAILED_CONNECTING       -4
#define FAILED_SENDING          -5

#endif