#include "commons.h"
#include "client_util.h"

int main() 
{
    int socket_desc = new_socket(8888, "127.0.0.1");
    if(socket_desc < 0) return socket_desc;
    request_hostname(socket_desc);
    receive_reply(socket_desc, STDOUT_FILENO);
    return 0;
}