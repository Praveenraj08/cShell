# cShell
A simple Shell( command line interpreter (CLI) ) in C.

When a command is typed,the shell creates a child process that executes the command you entered and then prompts for more user input when it has finished.

Each forked process is executed as a separate process to execute the command. The command is executed as a separate process with the help of execv().


