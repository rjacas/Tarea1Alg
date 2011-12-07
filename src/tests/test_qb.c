#define DEBUG
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/queue_buf.h"

#define N_ELEMS 8

void main(int argc, char **argv) {
    int fd, i;
    int foo[N_ELEMS];
    struct queue_buf *qb;

    fd = open("test_file", O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
  
    for (i = 0; i < N_ELEMS; i++) {
        foo[i] = rand();
        printf("Foo[%d]=%d\n", i, foo[i]);
    }

    write(fd, (void *)foo, sizeof(int)*N_ELEMS);

    close(fd);
    
    fd = open("test_file", O_RDWR);

    qb = qb_new(N_ELEMS);

    qb_refill(qb, fd);

    for (i = 0; i < N_ELEMS; i++) {
        printf("Extracted %d\n", qb_dequeue(qb));
    }

}