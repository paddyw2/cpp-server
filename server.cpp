#include "server.h"


/*
 * Constructor
 * Creates initial server socket on
 * specified port and address
 */
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
 * start_server()
 * Runs infinitely, waiting for and handling client
 * connections one at a time
 */
int server::start_server()
{
    // define variables
    char buffer[BUFFERSIZE];
    int error_flag;
    socklen_t clilen = sizeof(cli_addr);

    // enter infinite server loop
    while(1) {
        // create processor instance
        processor command_processor;
        cout << "Waiting for a client connection" << endl;
        // accept new client connection
        clientsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,&clilen);

        // error check
        if (clientsockfd < 0)
             error("ERROR on accept");

        while(1) {
            // clear buffer and read client input
            bzero(buffer,BUFFERSIZE);
            read_from_client(buffer,BUFFERSIZE-1);

            // notify server of successful message transfer
            // and process the client request
            cout << "Received client input" << endl;

            // if client quits, terminate connection
            if(strcmp((const char *)buffer, LOGOUT) == 0) {
                char message[] = "Quiting program\n";
                write_to_client(message, strlen(message)); 
                break;
            }

            // if client sends server kill command, quit server
            if(strcmp((const char *)buffer, KILL) == 0)
                stop_server();

            // else, send command to be processed
            vector<string> command_output = command_processor.parse(buffer);
            print_output_client(command_output);
        }
        // close client socket and loop
        // back to accept new connection
        close(clientsockfd);
    }
    return 0;
}

/*
 * Closes the server
 */
int server::stop_server()
{
    cout << "Closing server socket" << endl;
    close(sockfd);
    exit(EXIT_SUCCESS);
}

/*
 * Loops through the vector string and prints
 * each line to the client as the result of
 * their command
 */
int server::print_output_client(vector<string> output)
{
    // declare variables
    string newline;
    vector<string>::iterator itr;
    // print divider
    print_client_divider("Output");

    // initialize iterator
    itr = output.begin();

    // loop through vector
    while(itr != output.end()) {
        newline = *itr;
        char * line = (char *)newline.c_str();
        int length = strlen(line);
        // write a line to the client
        write_to_client(line, length);
        itr++;
    }

    // print divider
    print_client_divider("Command");
    return 0;
}

/*
 * Writes to client socket and checks
 * for errors
 */
int server::write_to_client(char * message, int length)
{
    int error_flag;
    error_flag = write(clientsockfd, message, length); 
    // error check
    if (error_flag < 0)
        error("ERROR writing to socket");
    return 0;
}

/*
 * Reads from client socket and checks
 * for errors
 */
int server::read_from_client(char * message, int length)
{
    int error_flag;
    error_flag = read(clientsockfd, message, length); 
    strip_newline(message, length);
    // error check
    if (error_flag < 0)
        error("ERROR reading from socket");
    return 0;
}

/*
 * Prints a divider to the client screen with
 * a header message specified by the paramter
 */
int server::print_client_divider(const char * message)
{
    char divider_start[] = "#------------------ ";
    char divider_end[] = " ------------------#\n";

    write_to_client(divider_start, strlen(divider_start)); 
    write_to_client((char *)message, strlen(message)); 
    write_to_client(divider_end, strlen(divider_end)); 
    return 0;
}

/*
 * Strip new line from string
 */
int server::strip_newline(char * input, int max)
{
    if(input[strlen(input)-1] == '\n')
        input[strlen(input)-1] = 0;

    if(input[strlen(input)-1] == '\r')
        input[strlen(input)-1] = 0;

    return 0;
}

/*
 * Error handler
 */
void server::error(const char *msg) {
    perror(msg); exit(1);
}
