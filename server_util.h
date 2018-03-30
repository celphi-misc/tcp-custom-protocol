// This file serves as the interface of the server utility methods
#ifndef __CELPHI_SERVER_UTIL_H
#define __CELPHI_SERVER_UTIL_H

#include "commons.h"

// Constants
#define HOSTNAME_LENGTH 2048

// Methods

// Create a new socket on the referred port number
// The return value is either an error code (negative)
// or a socket descriptor.
int new_socket(uint16_t port);

#endif