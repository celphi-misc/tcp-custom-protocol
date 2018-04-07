#include "commons.h"
#include "client_util.h"
#include "client_cli.h"
#include <pthread.h>

int client_should_stop=0;
int socket_desc;

void* start_listen(){
    printf("listen running\n");
    while(!client_should_stop)
    {
        // receive and output section
        // TODO: parse message and implement client status change 
        receive_reply(socket_desc, STDOUT_FILENO);
    }
    printf("client receive stop\n");
    pthread_exit(0);
}

int main() 
{
    int input_ret = show_welcome();
    switch (input_ret) 
    {
        case INPUT_CONNECT:
        {
            printf("Connecting .. \n");
            socket_desc = new_socket(8888, "127.0.0.1");
            if(socket_desc < 0) return socket_desc;
            printf("Connected.\n");
            break;
        }
        case INPUT_EXIT:
        {
            printf("Bye\n");
            return 0;
        }
    }

    pthread_t pid;
    pthread_create(&pid, NULL, start_listen, NULL);

    while(!client_should_stop) 
    {
        // Input and send section
        // TODO: implement client status and operations

        // Welcome Scene:
        request_hostname(socket_desc);
        // TODO: synchronize send thread and recv thread
        client_should_stop = 1;
    }
    pthread_join(pid, NULL);
    // request_send_message(socket_desc, 3, "Hello");
    // receive_reply(socket_desc, STDOUT_FILENO);
    return 0;
}