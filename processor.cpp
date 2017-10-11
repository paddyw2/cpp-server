#include "processor.h"

processor::processor()
{
    // create class
}

/*
 * opens a file in the current directoy by name
 * and returns its contents as an unsigned char *
 */
vector<string> processor::open_stdout(const char * command)
{
    // initialize variables
    int buff_size = LINESIZE;
    char line_buffer[buff_size];
    bzero(line_buffer, buff_size);
    vector<string> output;
    FILE *stdout_ptr;
    // open stdout output stream
    stdout_ptr = popen(command, "r");
    if(!stdout_ptr) {
        printf("File opening stream\n");
        exit(EXIT_FAILURE);
    }

    // pipe output line by line into the
    // string vector
    while (fgets(line_buffer, buff_size, stdout_ptr) != NULL) {
        output.push_back(line_buffer);
        cout << line_buffer;
        bzero(line_buffer, buff_size);
    }

    // close stream once all output
    // is gathered
    int error = pclose(stdout_ptr);

    // return stream output
    return output;
}

/*
 * Takes user command, checks it, executes it on
 * the server, the returns the output in the form
 * of a string vector, line per line
 */
vector<string> processor::parse(char * user_input)
{
    cout << "Parsing user input" << endl;
    vector<string>  command_output;
    // parse logic
    char stderr_redirect[] = " 2>&1\n";
    char command[strlen(stderr_redirect)+strlen(user_input)];
    bzero(command, sizeof(command));
    memcpy(command, user_input, strlen(user_input)-2);
    memcpy(command+strlen(user_input)-2,stderr_redirect, strlen(stderr_redirect));
    if(strcmp(command, HELP) == 0)
        command_output = get_help();
    else
        command_output = open_stdout(command);
    cout << "Finished parsing" << endl;
    // if bad command, no stdout output
    // stderr instead, so output is empty
    if(command_output.size() < 1)
        command_output.push_back("[no stdout output]\n");

    return command_output;
}

/*
 * Pushes standard help message to
 * a vector string and returns it
 */
vector<string> processor::get_help()
{
    vector<string> output;
    char line[] = "Help\nHere are some commands that you can use:\npwd: gets present working directory\n";
    output.push_back(line);
    return output;
}
    
