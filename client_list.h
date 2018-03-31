#ifndef __CELPHI_CLIENT_LIST
#define __CELPHI_CLIENT_LIST
#include "commons.h"

typedef struct
{
    int desc;
    struct sockaddr_in socket_addr;
} ClientEntry;

extern ClientEntry client_list[MAX_CLIENT];

// The result value is index of the new client
int add_client(int desc, struct sockaddr_in *p_socket_addr);
// This removes the client entry in the list by file descriptor
int remove_client(int desc);
// Find client socket addresses by file descriptor
// This stuff returns a pointer to a struct sockaddr_in
struct sockaddr_in *find_socket_addr(int desc);

#endif