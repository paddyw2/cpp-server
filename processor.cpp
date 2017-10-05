#include "processor.h"

processor::processor()
{
    // create class
}

/*
 * opens a file in the current directoy by name
 * and returns its contents as an unsigned char *
 */
char * processor::open_file(const char * command)
{
    // create and open file
    FILE *stdout_ptr;
    stdout_ptr = popen(command, "r");
    if(!stdout_ptr) {
        printf("File opening stream\n");
        exit(EXIT_FAILURE);
    }

    // close stream
    pclose(stdout_ptr);

    // return stream output
    char * fileBuffer = new char[10];
    return fileBuffer;
}

int processor::parse(char * user_input)
{
    cout << "Parsing user input" << endl;
    // logic
    const char * user_command = "pwd";
    char * output = open_file(user_command);
    cout << output << endl;
    cout << "Finished parsing" << endl;
    int return_val = 1;
    if(user_input[0] == 'q')
        return_val = 0;
    return return_val;
}
