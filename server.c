#include "server_util.h"

int server_should_stop = 0;

void* start_serve(void* client_desc) 
{
    service(*(int*)client_desc);
    pthread_exit(0);
}

int main(int argc, char **argv)
{
    int server_desc = new_socket(8888);
    if(server_desc < 0) return server_desc;
    while (!server_should_stop)
    {
        int client_desc;
        int accept_exitcode = accept_client(server_desc, &client_desc);
        if(accept_exitcode != SUCCEED_EXITCODE) return accept_exitcode;

        pthread_t  pid;
        pthread_create(&pid, NULL, start_serve, &client_desc);
        pthread_detach(pid);

        //service(client_desc);
    }
    return 0;
}