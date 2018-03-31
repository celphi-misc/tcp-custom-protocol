#include "client_list.h"

// Definition of the concrete client_list
ClientEntry client_list[MAX_CLIENT];

// TODO: Thread safety

// The result value is index of the new client
int add_client(int desc, struct sockaddr_in *p_socket_addr)
{
    int i;
    for(i = 0; i <= MAX_CLIENT; i++)
    {
        if(client_list[i].desc <= 0) break;
    }
    if(i == MAX_CLIENT)
        return MAX_CLIENT_EXCEEDED;
    client_list[i].desc = desc;
    client_list[i].socket_addr = *p_socket_addr;
    return i;
}

// This removes the client entry in the list by file descriptor
int remove_client(int desc)
{
    int i;
    for(i = 0; i <= MAX_CLIENT; i++)
        if(client_list[i].desc == desc) break;
    if(i == MAX_CLIENT)
        return CLIENT_NOT_FOUND;
    client_list[i].desc = 0;
    return i;
}

// Find client socket addresses by file descriptor
// This stuff returns a pointer to a struct sockaddr_in
struct sockaddr_in *find_socket_addr(int desc)
{
    int i;
    for(i = 0; i <= MAX_CLIENT; i++)
        if(client_list[i].desc == desc) break;
    if(i == MAX_CLIENT)
        return NULL;
    return &(client_list[i].socket_addr);
}