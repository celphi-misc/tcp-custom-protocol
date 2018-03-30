#include "commons.h"
#include "client_util.h"

int main() 
{
    int exit_code;
    // Initialize client name
    if((exit_code = init()) != SUCCEED_EXITCODE) return exit_code;

    // show welcome interface
    show_welcome();
    
    int socket_desc;
    if((exit_code = new_socket(&socket_desc, PORT)) != SUCCEED_EXITCODE) return exit_code;

    // TODO: CLI-design & recv implementation

    return 0;
}