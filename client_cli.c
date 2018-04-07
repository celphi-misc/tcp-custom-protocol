#include "client_cli.h"

char *s;

void cli_init()
{
    s = (char*)malloc(sizeof(char)*256);
}

int show_welcome() 
{
    cli_init();
    printf("+-----------------------------------------------+\n");
    printf("|                                               |\n");
    printf("|  Welcome to Celphi-Zephyr Web Client service  |\n");
    printf("|                                               |\n");
    printf("+-----------------------------------------------+\n");
    printf("|  IP: 127.0.0.1 | Current status: Unconnected  |\n");
    printf("+-----------------------------------------------+\n");
    printf("|  Options:                                     |\n");
    printf("|  1. Connect: \tenter \"connect\" or \"c\".         |\n");
    printf("|  2. Help: \tenter \"help\" or \"h\".            |\n");
    printf("|  3. Exit: \tenter \"exit\" or \"e\".            |\n");
    printf("+-----------------------------------------------+\n");
    return process_input_menu();
}

int process_input_menu() 
{
    scanf("%s", s);
#ifdef CLI_TEST
    printf("%d %d %d\n", strcmp(s, "connect"), strcmp(s, "help"), strcmp(s, "exit"));
#endif
    
    if(strcmp(s, "connect") ==0 || strcmp(s, "c")==0) return INPUT_CONNECT;
    else if(strcmp(s, "help") == 0 || strcmp(s, "h") == 0) return show_help();
    else if(strcmp(s, "exit") ==0 || strcmp(s, "e") == 0) return INPUT_EXIT;
    else {
        printf("Unrecognized command, please try again.\n");
        return process_input_menu();
    }
}

int show_help() 
{
    printf("+-----------------------------------------------+\n");
    printf("|                  Help Manual                  |\n");
    printf("|-----------------------------------------------|\n");
    printf("|  Welcome to Celphi-Zephyr web client service! |\n");
    printf("|    To use this web service properly, you may  |\n");
    printf("|  first choose to connect the server.          |\n");
    printf("|    Further service will provide you with more |\n");
    printf("|  options, such as send request to the server  |\n");
    printf("|  and chat with another client who connects to |\n");
    printf("|  the server.                                  |\n");
    printf("+-----------------------------------------------+\n");
    printf("|  You may try:                                 |\n");
    printf("|    hostname or hn : request the server's name |\n");
    printf("|    hostip or hi : request the server's ip     |\n");
    printf("|    hostname or hn : request the server's name |\n");
    printf("|    time : request time of server              |\n");
    printf("|    list : request the client list from server |\n");
    printf("|    (id)* : format to chat with another client |\n");
    printf("|       id is the number of client, * is your   |\n");
    printf("|       message.                                |\n");
    printf("|    exit : to quit or disconnect from server   |\n");
    printf("+-----------------------------------------------+\n"); 
    printf("|  Input anything to return to the upper level  |\n");
    printf("+-----------------------------------------------+\n"); 

    scanf("%s", s);
    return show_welcome();
}
