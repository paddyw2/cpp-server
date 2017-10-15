#include "server.h"

int main(int argc, char * argv[])
{
    cout << "Backdoor Server" << endl;
    server new_server(argc, argv);
    new_server.start_server();
    return 0;
}
