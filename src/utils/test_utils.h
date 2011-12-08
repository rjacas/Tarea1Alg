#ifndef _TEST_UTILS_
#define _TEST_UTILS_

/*
   Prints the integers contained in a file.

Params:
    fd(IN/OUT) - The file descriptor of the file to print.
    The position of the file descriptor will be altered.
*/
void print_file_integers(int fd);
#endif
