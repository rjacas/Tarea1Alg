#include <stdlib.h>
#include "queue_buf.h"

struct queue_buf *qb_new(int size) {
    struct queue_buf *q;

    q = (struct queue_buf *)malloc(sizeof(struct queue_buf));
    q->elems = (int *)malloc(sizeof(int) * size);
    q->size = size;
    q->first = q->n_elems = 0;
    return q;
}

int qb_refill(struct queue_buf *q, int fd) {
   int ret;

   ret = read(fd, (void *)q->elems, (q->size)*sizeof(int));

   q->n_elems = ret/sizeof(int);
   q->first = 0;
   return (q->n_elems == q->size)? TRUE : FALSE;
}

int qb_flush(struct queue_buf *q, int fd) {
    int ret, start, total;
    int *elems;

    start = 0;
    elems = q->elems;
    total = q->n_elems * sizeof(int);

    while (1) {
        ret = write(fd, (void *)q->elems, (q->n_elems)*sizeof(int));
        if (
            ((elems = elems + ret/(sizeof(int))) ;
    q->n_elems = q->first = 0;

    return ret;
}

int qb_enqueue(struct queue_buf *q, int new_elem) {
#ifdef DEBUG
    if (full(q)) {
        printf("Queue is full!");
        exit(0);
    }
#endif
    q->elems[n_elems++] = new_elem;
    
}

int qb_dequeue(struct queue_buf *q) {
#ifdef DEBUG
    if (empty(q)) {
        printf("No element to dequeue!\n");
        exit(1);
    }
#endif
    return q->elems[q->ptr++];
}

int qb_empty(struct queue_buf *q) {
    return (q->n_elems == q->ptr)? TRUE : FALSE;
}
      
int qb_full(struct queue_buf *q) {
    return (q->n_elems == q->size)? TRUE : FALSE;
}
