# cShell
A simple Shell( command line interpreter (CLI) ) in C.

To execute a command,the shell creates a child process that executes the command you entered and then prompts the user for more input when it has finished.

Each forked process is executed as a separate process to execute the command. The command is executed as a separate process with the help of execv().

For any error only one error message is shown.
