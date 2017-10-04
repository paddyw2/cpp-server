#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>

using namespace std;

class server
{
    public:
        server(int argc, char * argv[]);
        int start_server();
        int process_input(char * user_input);
        int stop_server();
        void error(const char *msg);


    private:
        int sockfd;
        int portno;
        struct sockaddr_in serv_addr;
        struct sockaddr_in cli_addr;
};
