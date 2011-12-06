#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/queue_buf.h"

#define DEBUG
#define N_ELEMS 8

void main(int argc, char **argv) {
    int fd, i;
    int foo[N_ELEMS];
    struct queue_buf *qb;

    fd = open("test_file", O_WRONLY | O_CREAT);
  
    for (i = 0; i < N_ELEMS; i++) {
        foo[i] = rand();
        printf("Foo[%d]=%d\n", i, foo[i]);
    }

    write(fd, (void *)foo, sizeof(int)*N_ELEMS);

    close(fd);
    
    fd = open("test_file", O_RDONLY);

    qb = queue_buf_new(N_ELEMS);

    refill_queue_buf(qb, fd);

    for (i = 0; i < N_ELEMS; i++) {
        printf("Extracted %d\n", dequeue(qb));
    }

}
