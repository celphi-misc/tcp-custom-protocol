#ifndef __ZEPHYR_SERVER_UTIL_H
#define __ZEPHYR_SERVER_UTIL_H

#include "commons.h"

// Constants
#define CLIENT_NAME_LENGTH 2048
#define PORT 8888

#define true 1
#define false 0

// Methods

// initialize client name and host name
int init();
/** Main routine of the client
 * int* socket_desc : a pointer of socket descriptor's id
 * int port         : default port number to the server
 * return an exit code
 **/   
int new_socket(int* socket_desc, int port);

void show_welcome();

#endif
