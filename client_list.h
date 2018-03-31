#ifndef __CELPHI_CLIENT_LIST
#define __CELPHI_CLIENT_LIST
#include "commons.h"

typedef struct
{
    int fd;
    struct sockaddr_in socket_addr;
} ClientEntry;

extern ClientEntry client_list[MAX_CLIENT];

#endif