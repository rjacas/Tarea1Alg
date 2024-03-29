#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../algorithms/m_mergesort.h"
#include "../utils/test_utils.h"

main(int argc, char **argv) {
    int fd1, fd2, k;
    off_t size;
    char *endptr = NULL;

    if (argc < 5) {
        printf("This test requires a file as the first argument, n_elems as the second, an aux file as the third and k as the fourth\n");
        exit(1);
    }

    size = atoll(argv[2]);
    
    fd1 = open(argv[1], O_RDONLY);
    print_file_integers(fd1);

    close(fd1);

    printf("Sorting...\n");
    fd1 = open(argv[1], O_RDWR);
    fd2 = open(argv[3], O_RDWR);

    k = atoi(argv[4]);

    m_mergesort(fd1, fd2, k, size);

    lseek(fd1, 0, SEEK_SET);
    print_file_integers(fd1);
    close(fd1);
    close(fd2);
}
