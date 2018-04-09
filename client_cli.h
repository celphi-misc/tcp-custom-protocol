#ifndef __ZEPHYR_CLIENT_CLI
#define __ZEPHYR_CLIENT_CLI
#include "commons.h"
#include "client_util.h"

#define INPUT_INVALID       -1
#define INPUT_EXIT          0
#define INPUT_CONNECT       1
#define INPUT_DISCONNECT    2

#define NAN             -1
#define IS_NUM          1

int show_welcome();

int show_welcome_connected(int socket_desc);

void show_nothing(int socket_desc, int out);

void show_help();

int process_input_menu();

int process_input_connected(int socket_desc);

#endif