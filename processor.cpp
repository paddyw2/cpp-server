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
    const char * user_command = (const char *)user_input;
    command_output = open_stdout(user_command);
    cout << "Finished parsing" << endl;
    // if bad command, no stdout output
    // stderr instead, so output is empty
    if(command_output.size() < 1)
        command_output.push_back("[no stdout output]\n");

    return command_output;
}
