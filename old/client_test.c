#include "commons.h"

void print_array_in_hex(unsigned char *array)
{
    int len = ntohl(*(int*)(array + 2));
    for(int i = 0; i < len + PROTOCOL_HEADER_LEN; i++)
    {
        printf("%2x ", array[i]);
        if(i%16 == 15) puts("");
    }
    puts("");
}

int main(int argc, char **argv)
{
    struct sockaddr_in server;
    char message[MESSAGE_LENGTH];
    char reply[MESSAGE_LENGTH];
    char ip_addr[21] = "127.0.0.1";

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        puts("Socket creation failed.");
        return FAILED_SOCKET_CREATION;
    }
    puts("Socket creation succeeded.");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip_addr);
    // TODO: Modify the htons arg
    server.sin_port = htons(8888);
    // Connect to the remote server
    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connect failed. Error");
        return FAILED_CONNECTING;
    }
    puts("Connected");

    while(1)
    {
        // // Send data
        // if(send(sock, message, strlen(message), 0) < 0)
        // {
        //     puts("Send failed");
        //     return FAILED_SENDING;
        // }

        // // Receive a reply from the server
        // if(recv(sock, reply, MESSAGE_LENGTH, 0) < 0)
        // {
        //     puts("recv failed");
        //     break;
        // }

        // puts("Server reply: ");
        // puts(reply);

        int message_len = request_time_msg(message);
        // Send data
        if(send(sock, message, message_len, 0) < 0)
        {
            puts("Send failed");
            return FAILED_SENDING;
        }
        // Receive a reply from the server
        if(recv(sock, reply, MESSAGE_LENGTH, 0) < 0)
        {
            puts("recv failed");
            break;
        }

        puts("Server reply: ");
        print_array_in_hex(reply);
        system("read a");
    }

    close(sock);
    return 0;
}