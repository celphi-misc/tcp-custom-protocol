#ifndef __ZEPHYR_CLIENT_CLI
#define __ZEPHYR_CLIENT_CLI
#include "commons.h"
#include "client_util.h"

#define INPUT_INVALID   0x000
#define INPUT_CONNECT   0x010
#define INPUT_EXIT      0X100

#define NAN             -1
#define IS_NUM          1

int show_welcome();

int show_welcome_connected(int socket_desc);

void show_nothing(int socket_desc, int out);

int show_help();

int process_input_menu();

int process_input_connected(int socket_desc);

void io_release();

#endif