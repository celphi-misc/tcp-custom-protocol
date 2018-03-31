#include "server_util.h"

int main(int argc, char **argv)
{
    int server_desc = new_socket(8888);
    if(server_desc < 0) return server_desc;
    int client_desc;
    int accept_exitcode = accept_client(server_desc, &client_desc);
    if(accept_exitcode != SUCCEED_EXITCODE) return accept_exitcode;
    service(client_desc);
    return 0;
}