all:
		gcc -g -Wall -o sh shell.c
clean:
		$(RM) fs
