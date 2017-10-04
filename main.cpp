#include "server.h"

int main(int argc, char * argv[])
{
    server new_server(argc, argv);
    new_server.start_server();
    return 0;
}
