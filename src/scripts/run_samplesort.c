#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../algorithms/alpha_samplesort.h"
#include "../utils/test_utils.h"

main(int argc, char **argv) {
    struct sort_results sr;
    int fd, t;
    char prefix[2];
    off_t n;
    char *c;
    
    if (argc < 5) {
        printf("This test requires a file as the first argument\n");
        exit(1);
    }

    if((fd = open(argv[1], O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO))== -1){
      perror("aca");
      exit(1);
    }
    
    n = atoll(argv[2]);
    c = argv[3];
    t = atoi(argv[4]);
     
    sr = alpha_samplesort(fd, n, c, t);
    printf("Total accesses: %lld\n", sr.io_acc);
    printf("Total random acc: %lld\n", sr.io_rand);
} 
