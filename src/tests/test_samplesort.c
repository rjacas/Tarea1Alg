#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../algorithms/alpha_samplesort.h"
#include "../utils/test_utils.h"

main(int argc, char **argv) {
    int foo[N_ELEMS];
    int i,n,t, fd;
    int size; 
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
    t = atoi(argv[4]);
    c = argv[3]; 
    //print_file_integers(fd);

    //close(fd);

    //printf("Sorting...\n");
    //fd = open(argv[1], O_RDWR);
    alpha_samplesort(fd, n,c, 2);

    //lseek(fd, 0, SEEK_SET);
    //print_file_integers(fd);
    close(fd);
}
