#define DEBUG
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/queue_buf.h"
#include "../utils/test_utils.h"

#define N_ELEMS 8

main(int argc, char **argv) {
    int fd;
    struct queue_buf *qb;
    if (argc < 2) {
        printf("This test requires a file as the first argument\n");
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);
    printf("\n========= Testing flush =========\n");

    print_file_integers(fd);

    lseek(fd, 0, SEEK_SET);

    qb = qb_new(N_ELEMS);
    qb_refill(qb, fd);

    close(fd);

    fd = open("test_file2", O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
  
    qb_flush(qb, fd);

    lseek(fd, 0, SEEK_SET);

    printf("\n========= Copy should be the same...  =========\n");
    print_file_integers(fd);

    close(fd);

}
