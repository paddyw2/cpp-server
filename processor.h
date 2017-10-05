#include <stdio.h>
#include <stdlib.h>

#include <iostream>

using namespace std;

class processor
{
    public:
        processor();
        int parse(char * user_input);
        char * open_file(const char * command);

};
