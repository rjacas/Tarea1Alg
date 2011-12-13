#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../algorithms/alpha_samplesort.h"
#include "../utils/test_utils.h"

#define N_ELEMS 24
main(int argc, char **argv) {
    int foo[N_ELEMS];
    int i, fd;
    int size;
    char *endptr = NULL;

    if (argc < 2) {
        printf("This test requires a file as the first argument\n");
        exit(1);
    }

    if((fd = open(argv[1], O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO))== -1){
      perror("aca");
      exit(1);
    }
    
    //print_file_integers(fd);

    //close(fd);

    //printf("Sorting...\n");
    //fd = open(argv[1], O_RDWR);
    alpha_samplesort(fd, N,"", 2);

    //lseek(fd, 0, SEEK_SET);
    //print_file_integers(fd);
    close(fd);
}
