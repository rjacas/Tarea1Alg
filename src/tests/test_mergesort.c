#define DEBUG
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../algorithms/m_mergesort.h"
#include "../utils/test_utils.h"
#define N_ELEMS 8

main(int argc, char **argv) {
    int foo[N_ELEMS];
    int i, fd;
    int size;
    char *endptr = NULL;

    if (argc < 2) {
        printf("This test requires a file as the first argument\n");
        exit(1);
    }

    fd = open(argv[1], O_RDWR);
    m_mergesort(fd);

    printf("Getting %d random integers from file...\n");
    size = read(fd, (void *)foo, sizeof(int) * N_ELEMS);

    for (i = 0; i < N_ELEMS; i++) {
        printf("Random  number: %d\n", foo[i]);
    }
}
