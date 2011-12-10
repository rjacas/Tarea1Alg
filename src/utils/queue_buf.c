#ifdef DEBUG
#include <stdio.h>
#endif
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

int qb_refill_max(struct queue_buf *q, int fd, int max_bytes) {
   int ret;

   if (max_bytes >= q->size * sizeof(int)) return qb_refill(q, fd);

   ret = read(fd, (void *)q->elems, max_bytes);

   q->n_elems = ret/sizeof(int);
   q->first = 0;

   return (q->n_elems == q->size)? TRUE : FALSE;
}


int qb_flush(struct queue_buf *q, int fd) {
    int ret, idx, size;

    idx = ret = 0;
    size = q->n_elems * sizeof(int);

    /* Y PAF */
    while ((ret = write(fd, &(q->elems[idx]), size - idx)) > 0 && (size - idx) > 0) idx += ret;

    q->n_elems = q->first = 0;

    return (ret < 0)? ret : idx;
}

void qb_enqueue(struct queue_buf *q, int new_elem) {
#ifdef DEBUG
    if (qb_full(q)) {
        printf("Queue is full!");
        exit(0);
    }
#endif
    q->elems[q->n_elems++] = new_elem;
    
}

int qb_dequeue(struct queue_buf *q) {
#ifdef DEBUG
    if (qb_empty(q)) {
        printf("No element to dequeue!\n");
        exit(1);
    }
#endif
    q->n_elems--;
    return q->elems[q->first++];
}

void qb_get_array(struct queue_buf *q, int **arr, int *size) {

    *arr = q->elems + q->first;
    *size = q->n_elems;
}

int qb_empty(struct queue_buf *q) {
    return (q->n_elems == 0)? TRUE : FALSE;
}
      
int qb_full(struct queue_buf *q) {
    return (q->n_elems == q->size)? TRUE : FALSE;
}
