#ifndef __ZEPHYR_SERVER_UTIL_H
#define __ZEPHYR_SERVER_UTIL_H

#include "commons.h"

// Methods

// initialize client name and host name
int init();
// Create a new socket on the referred port number and IP address
// The return value is either an error code (negative)
// or a socket descriptor 
int new_socket(uint16_t port, const char *ip_addr);

#endif
