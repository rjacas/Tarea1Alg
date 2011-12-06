#include <stdlib.h>
#include "queue_buf.h"

struct queue_buf *queue_buf_new(int size) {
    struct queue_buf *q;

    q = (struct queue_buf *)malloc(sizeof(struct queue_buf));
    q->elems = (int *)malloc(sizeof(int) * size);
    q->size = size;
    q->ptr = 0;
    q->n_elems = 0;
    return q;
}

int refill_queue_buf(struct queue_buf *q, int fd) {
   ssize_t ret;

   ret = read(fd, (void *)q->elems, (q->size)*sizeof(int));

   q->n_elems = ret/sizeof(int);
   q->ptr = 0;
   return (q->n_elems == q->size)? TRUE : FALSE;
}

int dequeue(struct queue_buf *q) {
#ifdef DEBUG
    if (empty(q)) {
        printf("No element to dequeue!\n");
        exit(1);
    }
#endif
    return q->elems[q->ptr++];
}

int empty(struct queue_buf *q) {
    return (q->n_elems == q->ptr)? TRUE : FALSE;
}
            
