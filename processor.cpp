#include "processor.h"

processor::processor()
{
    // create class
}

int processor::parse(char * user_input)
{
    cout << "Parsing user input" << endl;
    // logic
    cout << "Finished parsing" << endl;
    int return_val = 1;
    if(user_input[0] == 'q')
        return_val = 0;
    return return_val;
}
