#include "processor.h"

/*
 * Constuctor: Sets the intial present
 * working directory
 */
processor::processor()
{
    // create class and set
    // starting working directory
    bzero(current_dir, BUFFER_SIZE);
    getcwd(current_dir, BUFFER_SIZE);
    cout << current_dir << endl;
    snapshot_exists = 0;
}

/*
 * Takes a local file path and returns
 * 1 if it is a regular file, and 0
 * if it is something else (such as a
 * directory or sym link)
 */
int processor::check_if_file(const char * filename)
{
    // create a buffer for full string path
    // and zero it
    char full_path[strlen(filename) + strlen(current_dir) + 1];
    bzero(full_path,strlen(filename) + strlen(current_dir) + 1); 
    // copy base path to buffer
    memcpy(full_path, current_dir, strlen(current_dir));
    // add trailing slash
    full_path[strlen(current_dir)] = '/';
    // now copy filename to end, removing the newline character
    memcpy(full_path+strlen(current_dir) + 1, filename, strlen(filename)-1);

    // create stat structure
    struct stat filename_stat;
    // set its file path
    stat(full_path,&filename_stat);
    // check if regular file and return status
    int status = S_ISREG(filename_stat.st_mode);
    return status;
}

/*
 * Takes a temporary snapshot and saves it
 * as the latest snapshot
 */
vector<string> processor::save_snapshot(vector<string> new_snapshot)
{
    // clear old snapshot
    snapshot.clear();
    string current_line;
    vector<string>::iterator itr;

    // initialize iterator
    itr = new_snapshot.begin();

    // loop through vector, duplicating
    // values
    while(itr != new_snapshot.end()) {
        current_line = *itr;
        snapshot.push_back(current_line);
        itr++;
    }

    // indicate that a snapshot has been saved
    snapshot_exists = 1;

    // return success message
    vector<string> success;
    success.push_back("Snapshot saved\n");
    return success;
}


/*
 * Compares a temporary snapshot of the current
 * directory status with the saved snapshot
 * Any changes, additions, or deletions are
 * returned in a vector<string> to print to
 * the client as a status
 */
vector<string> processor::create_diff(vector<string> file_list)
{
    vector<string> diff;
    vector<string>::iterator itr_list;
    vector<string>::iterator itr_snap;
    string current_line;
    string snap_line;
    int file_found;

    // first check if snapshot exists
    if(snapshot_exists == 0) {
        diff.push_back("No snapshot exists\n");
        return diff;
    }

    /* Check for added or new files */
    // initialize iterator
    itr_list = file_list.begin();

    // loop through vector, duplicating
    // values
    while(itr_list != file_list.end()) {
        // get first filename
        file_found = 0;
        current_line = *itr_list;
        itr_snap = snapshot.begin();
        while(itr_snap != snapshot.end()) {
            // search for file name in
            // current snapshot
            snap_line = *itr_snap;
            if(current_line == snap_line) {
                // if found a match, check if
                // hashes are the same
                file_found = 1;
                snap_line = *(itr_snap+1);
                current_line = *(itr_list+1);
                if(snap_line == current_line) {
                    // file is unchanged
                } else {
                    // file changed
                    diff.push_back(*itr_list);
                    diff.push_back(" --> File changed\n");
                }
            } else {
                // continue searching
            }
            // continue to next snapshot filename
            itr_snap += 2;
        }
        if(file_found == 0) {
            // must be a new file
            diff.push_back(*itr_list);
            diff.push_back(" --> File added\n");
           
        }
        // increment over hashes
        // to next file in file list
        itr_list += 2;
    }

    /* Check for deleted files */
    // now all current files have been checked for changes
    // we must check for deletions
     
    itr_snap = snapshot.begin();
    // loop through old snapshot
    while(itr_snap != snapshot.end()) {
        file_found = 0;
        snap_line = *itr_snap;
        itr_list = file_list.begin();
        // check for each file in new current
        // directory state
        while(itr_list != file_list.end()) {
            current_line = *itr_list;
            if(current_line == snap_line) {
                file_found = 1;
            }
            itr_list += 2;
        }
        // if any file is not found, then
        // it must have been deleted
        if(file_found == 0) {
            // file must be deleted
            diff.push_back(*itr_snap);
            diff.push_back(" --> File deleted\n");
        }
        itr_snap += 2;
    }
    // check if nothing to report
    if(diff.size() < 1)
        diff.push_back("No changes\n");

    // return a vector with the affected
    // files and their status
    return diff;
}

/*
 * Takes a list of current files in the current
 * directory as input and for each regular file
 * it creates a sha256 hash digest
 * The filename and hashes are stored in the list
 * as follows:
 * filename1, hash1, filename2, hash2, ...
 * The result is returned and can be used as
 * either the new snapshot to be saved, or as a
 * comparision snapshot for the create_diff()
 * function
 */
vector<string> processor::create_snapshot(vector<string> file_list)
{
    cout << "Preparing..." << endl;
    // declare variables
    string current_file;
    vector<string> local_snapshot;
    vector<string>::iterator itr;

    // initialize iterator
    itr = file_list.begin();

    // loop through vector
    while(itr != file_list.end()) {
        current_file = *itr;
        // check if current file is a directory
        if(check_if_file(current_file.c_str()) != 1) {
            cout << "Detecting!!!!!" << endl;
            cout << check_if_file(current_file.c_str())  << endl;
            itr++;
            continue;
        }
        // to do 
        char * file = (char *)current_file.c_str();
        // remove new line
        int file_length = strlen(file)-1;
        // generate hash of that file
        char openssl[] = "openssl sha256 ";
        char command[strlen(file) + strlen(openssl)];
        bzero(command, strlen(file) + strlen(openssl));

        // concatenate strings
        memcpy(command, openssl, strlen(openssl));
        memcpy(command+strlen(openssl), file, file_length);

        vector<string> hash_value = open_stdout(command);
        // add filename and hash to data structure
        local_snapshot.push_back(current_file);
        local_snapshot.push_back(hash_value.front());
        itr++;
    }

    return local_snapshot;
}

vector<string> processor::change_dir(char * command)
{
    vector<string> output;
    int dir_size = strlen(command)+strlen(current_dir);
    char directory[dir_size];
    bzero(directory, dir_size);
    cout << "###--- Custom ---###" << endl;
        cout << current_dir << endl;
    // check if valid cd command
    // must specific a directory
    if(command[2] != ' ') {
        output.push_back("Invalid command\n");
        return output;
    } else if(command[3] == '/') {
        // complete path given, so simply
        // replace previous cwd
        memcpy(directory, command+3, strlen(command)-3);
    } else {
        // relative path given, so append
        // to cwd
        cout << current_dir << endl;
        memcpy(directory, current_dir, strlen(current_dir));
        directory[strlen(current_dir)] = '/';
        memcpy(directory+strlen(current_dir)+1, command+3, strlen(command)-3);
    }

    // update current working directory
    bzero(current_dir, BUFFER_SIZE);
    memcpy(current_dir, directory, strlen(directory));

    // change to that directory
    chdir(directory);
    output.push_back("New directory: ");
    output.push_back(directory);
    output.push_back("\n");
    return output;
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
        cout << "File opening stream" << endl;
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
 * Takes user command, checks it, and
 * delegates to a particular function
 * It returns the output in the form
 * of a string vector, line per line
 */
vector<string> processor::parse(char * user_input)
{
    cout << "Command: " << user_input << endl;
    vector<string>  command_output;

    // direct stderr to stdin to get a verbose
    // reponse
    char stderr_redirect[] = " 2>&1\n";

    // concatenate user command with stderr
    // redirection to get a full command
    int size1 = strlen(stderr_redirect);
    int size2 = strlen(user_input);
    char command[size1+size2+1];

    // copy the strings
    bzero(command, size1+size2+1);
    memcpy(command, user_input, size2);
    memcpy(command+size2,stderr_redirect, size1);

    // main parsing logic where the user
    // input is processed
    if(strncmp(user_input, "help", 5) == 0) {
        // print help message
        command_output = get_help();
    } else if(strncmp(user_input, "cd", 2) == 0) {
        // delegate processing to change_dir()
        command_output = change_dir(user_input);
    } else if(strncmp(user_input, "snap", 5) == 0) {
        // get a list of current directory files
        // and create a snapshot from them
        vector<string> file_list = open_stdout("ls");
        command_output = create_snapshot(file_list);
        command_output = save_snapshot(command_output);
    } else if(strncmp(user_input, "diff", 5) == 0) {
        // get a list of current directory files
        // and create a diff status
        vector<string> file_list = open_stdout("ls");
        vector<string> local_snap = create_snapshot(file_list);
        command_output = create_diff(local_snap);
    } else {
        // if not covered by the above cases, simply
        // execute the command in the shell
        command_output = open_stdout(command);
    }

    // if command finishes silently, let
    // user know
    if(command_output.size() < 1)
        command_output.push_back("[no output]\n");

    return command_output;
}

/*
 * Pushes standard help message to
 * a vector string and returns it
 */
vector<string> processor::get_help()
{
    vector<string> output;
    char line[] = HELP_TEXT;
    output.push_back(line);
    return output;
}
