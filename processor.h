#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include <vector>
#include <string>
#include <iostream>

#include "commands.h"

#define LINESIZE 512
#define BUFFER_SIZE 256

using namespace std;

class processor
{
    public:
        processor();
        vector<string> parse(char * user_input);
        vector<string> open_stdout(const char * command);
        vector<string> get_help();
        vector<string> change_dir(char * command);
        vector<string> create_snapshot(vector<string> file_list);
        vector<string> save_snapshot(vector<string> new_snapshot);
        vector<string> create_diff(vector<string> file_list);
        int check_if_file(const char * filename);

    private:
        char current_dir[BUFFER_SIZE];
        vector<string> snapshot;
        int snapshot_exists;
};
