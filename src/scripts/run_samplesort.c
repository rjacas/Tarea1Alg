#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../algorithms/alpha_samplesort.h"
#include "../utils/test_utils.h"

main(int argc, char **argv) {
    struct sort_results sr;
    int fd,k;
    off_t size;
    char *endptr = NULL;

    if (argc < 4) {
        printf("This test requires a file as the first argument, n_elems as the second and k as the third\n");
        exit(1);
    }

    size = atoll(argv[2]);
    
    fd = open(argv[1], O_RDWR);

    k = atoi(argv[3]);

    sr = s_samplesort(fd, size,"",k);

    printf("Total accesses: %lld\n", sr.io_acc);
    printf("Total random acc: %lld\n", sr.io_rand);
    close(fd);
}
