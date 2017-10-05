#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "processor.h"

#include <vector>
#include <string>
#include <iostream>

#define BUFFERSIZE 512

using namespace std;

class server
{
    public:
        server(int argc, char * argv[]);
        int start_server();
        int process_input(char * user_input);
        int stop_server();
        int print_output_client(vector<string> output);
        int write_to_client(char * message, int length);
        int read_from_client(char * message, int length);
        int print_client_divider(const char * message);
        void error(const char *msg);


    private:
        int sockfd;
        int clientsockfd;
        int portno;
        struct sockaddr_in serv_addr;
        struct sockaddr_in cli_addr;
};
