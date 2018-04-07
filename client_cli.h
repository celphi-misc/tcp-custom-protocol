#ifndef __ZEPHYR_CLIENT_CLI
#define __ZEPHYR_CLIENT_CLI
#include "commons.h"

#define INPUT_INVALID   0
#define INPUT_CONNECT   1
#define INPUT_EXIT      2


int show_welcome();

int process_input_menu();

int show_help();

#endif