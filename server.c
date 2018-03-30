#include "server_util.h"

int main(int argc, char **argv)
{
    int server_socket = new_socket(8888);
    int client_socket;
    char *received = receive(server_socket, &client_socket);
    if(received) service(server_socket, received);
    return 0;
}