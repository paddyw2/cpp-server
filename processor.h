#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <iostream>

#define LINESIZE 512

using namespace std;

class processor
{
    public:
        processor();
        vector<string> parse(char * user_input);
        vector<string> open_stdout(const char * command);
};
