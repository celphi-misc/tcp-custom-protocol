#include "client_cli.h"
// #define CLI_TEST

char *s;
int io_lock;
int client_should_stop=0;
int socket_desc;

void io_locked() 
{
    io_lock = 1;
}

void io_release() 
{
    io_lock = 0;
}

int io_should_wait() 
{
    if (io_lock == 1) return 1;
    else return 0;
}

void cli_init()
{
    s = (char*)malloc(sizeof(char)*256);
    io_lock = 0;
}

void* start_listen()
{
#ifdef PTHREAD_TEST
    printf("client listen running\n");
#endif
    while(!client_should_stop)
    {
        show_nothing(socket_desc, STDOUT_FILENO);
    }
#ifdef PTHREAD_TEST
    printf("client receive stop\n");
#endif
    pthread_exit(0);
}

int cli_connect() 
{   
    printf("Connecting .. \n");
    socket_desc = new_socket(8888, "127.0.0.1");
    if(socket_desc < 0){
        printf("Socket connect failed, please try again.\n");
        return process_input_menu();
    }
    else 
    {
        printf("Connected.\n");

        pthread_t pid;
        pthread_create(&pid, NULL, start_listen, NULL);

        int ret = show_welcome_connected(socket_desc);
        if(ret == INPUT_DISCONNECT) {
            pthread_cancel(pid);
            return show_welcome();
        }
        else 
            return INPUT_INVALID;
    }
}  

int process_input_menu() 
{
    scanf("%s", s);
#ifdef CLI_TEST
    printf("%d %d %d\n", strcmp(s, "connect"), strcmp(s, "help"), strcmp(s, "exit"));
#endif
    
    if(strcmp(s, "connect") ==0 || strcmp(s, "c")==0) return cli_connect();  
    else if(strcmp(s, "help") == 0 || strcmp(s, "h") == 0) 
    {
        show_help();
        return show_welcome();
    }
    else if(strcmp(s, "exit") ==0 || strcmp(s, "e") == 0) 
    {
        client_should_stop = 1;
        return INPUT_EXIT;
    }
    else 
    {
        printf("Unrecognized command, please try again.\n");
        return process_input_menu();
    }
}

void show_help() 
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
    printf("|       example:                                |\n");
    printf("|         (4)Hello! How are you ?               |\n");
    printf("|    exit : to quit or disconnect from server   |\n");
    printf("+-----------------------------------------------+\n"); 
    printf("|  Input anything to return to the upper level  |\n");
    printf("+-----------------------------------------------+\n"); 

    scanf("%s", s);
}

int show_welcome_connected(int socket_desc) 
{
    printf("+-----------------------------------------------+\n");
    printf("|                                               |\n");
    printf("|  Welcome to Celphi-Zephyr Web Client service  |\n");
    printf("|                                               |\n");
    printf("+-----------------------------------------------+\n");
    printf("|  IP: 127.0.0.1 | Current status:  Connected   |\n");
    printf("+-----------------------------------------------+\n");
    printf("|  Options:                                     |\n");
    printf("|  1. Get hostname : enter \"hostname\" or \"hn\"   |\n");
    printf("|  2. Get host IP :  enter \"hostip\" or \"hi\"     |\n");
    printf("|  3. Get time :     enter \"time\"               |\n");
    printf("|  4. Get user list: enter \"list\"               |\n");
    printf("|  5. Send message : use format \"(id)*\"         |\n");
    printf("|  6. Disconnect :   enter \"exit\"               |\n");
    printf("+-----------------------------------------------+\n");
    return process_input_connected(socket_desc);
}

void show_nothing(int socket_desc, int out) 
{
    receive_reply(socket_desc, STDOUT_FILENO);  
    io_release();
}

// format : "(id)message"
int check_format(char* s, int* chat_id, int* mesg_start)
{
    int p = 0;
    while (s[p] == ' ') p++;
    if(s[p++] != '(')
    {
        chat_id = NULL;
        mesg_start = NULL;
        return NAN;
    }
    while (s[p] == ' ') p++;
    int temp = 0;
    while(s[p] >='0' && s[p] <= '9')
    {
        temp = temp*10 + s[p] - '0';
        p++;
    }
    while (s[p] == ' ') p++;
    if(s[p++]!= ')')
    {
        chat_id = NULL;
        mesg_start = NULL;
        return NAN;
    }
    while (s[p]==' ') p++;
    if(p==strlen(s)) 
    {
        chat_id = NULL;
        mesg_start = NULL;
        return NAN;
    }
    *chat_id = temp;
    *mesg_start = p;
    return IS_NUM;
}

void cli_input(char* s){
    char c = ' ';
    int length = 0;
    while(1){
        scanf("%c", &c);
        if(c=='\n') break;
        s[length++] = c;
    }
    s[length] = 0;
}

int process_input_connected(int socket_desc) 
{
    while (io_should_wait());
    cli_input(s);
    io_locked();

    if(strcmp(s, "\n" )==0 || strcmp(s, "")==0){
        io_release();
    }
    else if (strcmp(s, "help")==0) {
        show_help();
        io_release();
        return show_welcome_connected(socket_desc);
    }
    else if(strcmp(s, "hostname")==0 || strcmp(s, "hn") ==0) 
    {
#ifdef CLI_TEST
        printf("requesting hostname\n");
#endif
        int ret;
        do 
        {
            ret=request_hostname(socket_desc);
#ifdef CLI_TEST
            printf("%d\n", ret);
#endif
        } while (ret < 0);
    }   
    else if (strcmp(s, "hostip")==0 || strcmp(s, "hi")==0) 
    {
        int ret;
        do 
        {
            ret=request_host_ip(socket_desc);
#ifdef CLI_TEST
            printf("%d\n", ret);
#endif
        } while (ret < 0);
    }
    else if (strcmp(s, "time") == 0) 
    {
#ifdef CLI_TEST
        printf("requesting time\n");
#endif
        int ret;
        do 
        {
            ret = request_time(socket_desc);
#ifdef CLI_TEST
            printf("%d\n", ret);
#endif
        } while (ret < 0);
    }
    else if (strcmp(s, "list")==0) 
    {
        int ret;
        do 
        {
            ret = request_listing_clients(socket_desc);
#ifdef CLI_TEST
            printf("%d\n", ret);
#endif
        } while (ret < 0);
    }
    else if (strcmp(s, "exit")==0)
    {
        request_disconnect(socket_desc);
        close(socket_desc);
        printf("Disconnected.\n");
        return INPUT_DISCONNECT;
    }
    else 
    {
        int chat_id = 0;
        int mesg_start;
        int is_mesg = check_format(s, &chat_id, &mesg_start);
        if(is_mesg == NAN) 
        {
            printf("Unrecognized input \"%s\", please try again.\n", s);
            io_release();
        } 
        else 
        {
            request_send_message(socket_desc, chat_id, s+mesg_start);
        }
    }
    return process_input_connected(socket_desc);
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