#define LOGOUT      "logout"
#define CD          "cd"
#define KILL        "off"
#define HELP        "help"
#define CD_INFO     "Format: cd [directory path]\nAccepts local paths and .. but does not accept ~\n"
#define PWD_INFO    "pwd: gets present working directory\n"
#define LS_INFO     "ls: lists the files in the current directory\n"
#define CP_INFO     "cp <file1> <file2>: copies file1 to file2\n"
#define CAT_INFO    "cat <file>: returns the contents of the file\n"
#define RM_INFO     "rm <file>: deletes the file\n"
#define MV_INFO     "mv <file1> <file2>: renames file1 to file2\n"
#define WHO_INFO    "who: displays currently logged in users\n"
#define PS_INFO     "ps: show currently running processes\n"
#define OFF_INFO    "Format: off\nTerminates the remote server\n"
#define LOGOUT_INFO "Format: logout\nTerminates your connection to the remote server\n"
#define HELP_INFO   "Format: help, or help [command]\nPrints either the standard help " \
                    "message, or a specific help message for the specified command\n" \
                    "For most standard UNIX-like commands, the output comes from man\n"
#define SNAP_INFO   "takes a snapshot of all the files in the current directory and " \
                    "save it in memory the snapshot should only include the filenames " \
                    "and hashes of the files the snapshot should survive client " \
                    "disconnecting and reconnecting later\n"
#define DIFF_INFO   "compare the contents of the current directory to the saved snapshot, " \
                    "and report  differences (deleted files, new files and changed files) " \
                    "this does not need to be recursive\n"
#define HELP_TEXT   "Here are some commands that you can use:\n" \
                    "  pwd: gets present working directory\n" \
                    "  ls: lists the files in the current directory\n" \
                    "  cd <dir>: changes to the specified directory\n" \
                    "  cp <file1> <file2>: copies file1 to file2\n" \
                    "  mv <file1> <file2>: renames file1 to file2\n" \
                    "  rm <file>: deletes file\n" \
                    "  ps: shows the current processes\n" \
                    "  who: shows the current logged in users\n" \
                    "  snap: takes a snapshot of files in current directory\n" \
                    "  diff: compares the saved snapshot to the current directory state\n" \
                    "  off: terminates the server program\n" \
                    "  logout: terminates your connection to the server\n" \
                    "  help [command]: prints a more detailed help message\n" \
                    "  help: prints this message\n"
