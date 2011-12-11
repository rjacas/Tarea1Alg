#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/queue_buf.h"
#include "../utils/test_utils.h"

/* 
   Tests that don't really fit anywhere.
*/

main(int argc, char **argv) {
    int fd;

    if (argc < 2) {
        printf("This test requires a file as the first argument\n");
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);
    printf("\n========= Testing print_file_integers =========\n");

    print_file_integers(fd);

    close(fd);
}


