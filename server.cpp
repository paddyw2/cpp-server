#include "server.h"

server::server(int argc, char * argv[])
{
    // check command line arguments
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    // create client socket and check for errors
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
       error("ERROR opening socket");

    // convert argument to port number
    // and check for errors
    portno = atoi(argv[1]);
    if(portno < 0)
       error("ERROR invalid port number");

    // clear structures and set to chosen values
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // bind socket to chosen address and port
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
             error("ERROR on binding");

    // start listening for connections on the
    // created socket
    listen(sockfd,5);
}

/*
 * Runs infinitely, waiting for and handling client
 * connections one at a time
 */
int server::start_server()
{
    // define variables
    int newsockfd;
    char buffer[256];
    int n;
    socklen_t clilen = sizeof(cli_addr);

    // enter infinite server loop
    while(1) {
        // accept new client connection
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,&clilen);

        // error check
        if (newsockfd < 0)
             error("ERROR on accept");

        // clear buffer and read client input
        bzero(buffer,256);
        n = read(newsockfd,buffer,255);

        // error check
        if (n < 0)
            error("ERROR reading from socket");

        // notify server of successful message transfer
        // and process the client request
        printf("Received client input\n");
        process_input(buffer);
        // notify client of successfull message transfer
        n = write(newsockfd,"I got your message\n",19);

        // error check
        if (n < 0)
            error("ERROR writing to socket");

        // close client socket and loop
        // back to accept new connection
        close(newsockfd);
    }
    return 0;
}

int server::stop_server()
{
    cout << "Closing server socket" << endl;
    close(sockfd);
    return 0;
}

int server::process_input(char * client_input)
{
    cout << "Processing..." << endl;
    return 0;
}

/*
 * Error handler
 */
void server::error(const char *msg) {
    perror(msg);
    exit(1);
}
