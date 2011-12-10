#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../algorithms/m_mergesort.h"
#include "../utils/test_utils.h"

main(int argc, char **argv) {
    int i, fd;
    int size;
    char *endptr = NULL;

    if (argc < 2) {
        printf("This test requires a file as the first argument\n");
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);
    
    print_file_integers(fd);

    close(fd);

    printf("Sorting...\n");
    fd = open(argv[1], O_RDWR);
    m_mergesort(fd);

    lseek(fd, 0, SEEK_SET);
    print_file_integers(fd);
    close(fd);
}
