#include "commons.h"
#include "client_util.h"
#include "client_cli.h"

int client_should_stop=0;
int socket_desc;

void* start_listen()
{
#ifdef PTHREAD_TEST
    printf("client listen running\n");
#endif
    while(!client_should_stop)
    {
        // receive and output section
        // TODO: parse message and implement client status change 
        show_nothing(socket_desc, STDOUT_FILENO);
    }
#ifdef PTHREAD_TEST
    printf("client receive stop\n");
#endif
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

    input_ret = show_welcome_connected(socket_desc);
    if(input_ret != INPUT_EXIT) {
        return -1;
    } else 
    {
        printf("Bye\n");
        pthread_cancel(pid);        
    }
    pthread_join(pid, NULL);
    // request_send_message(socket_desc, 3, "Hello");
    // receive_reply(socket_desc, STDOUT_FILENO);
    return 0;
}