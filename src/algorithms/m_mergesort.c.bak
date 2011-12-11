#include "m_mergesort.h"

#include "../utils/queue_buf.h"
#include "../utils/priority_queue.h"

/* exit queue should be of some bigger size (k * l * B  */

int N = 101;//24;
int n = 51;//12; //ceil (N/B)
int B = 2;
int M = 24;
int m = 12; //  M/B
#define k 3
int l = 4; // = m / k = length of buffers, in blocks
int min_size = 8; //l * B;

//replace k with 3
struct queue_buf *bufs[3]; 
struct queue_buf *ex;
struct priority_queue *heap;
int parity;

void m_mergesort(int fd){
    int i, aux, fd2;

    fd2 = open("test_file2", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    heap = pq_new(k);
    for( i = 0; i < k; i++){
        bufs[i] = qb_new(l * B);
    }
    ex = qb_new(l * B);  
    mergesort(fd, fd2, 0, 0, N);
    pq_free(heap);
} 

void mergesort(int fd1, int fd2, int level, int start, int size) {
    int i;
    int done[k];
    int q_size[k];
    int new_size, final_size; 
    int written, aux, aux2;
    int value;
    int qb_idx;
    int fd_in, fd_out;

    /* This is done only because this would be the only use
       of math.h . If you end up including math.h, change this
       to ceil(size/k) */
    new_size = ( size + k - 1 ) / k;
    aux = size;
    aux2 = (new_size <= min_size) ? l * B : new_size;

    for (i = 0; i < k; i++) {
        done[i] = 0;

        if (aux > 0) {
            if ((aux - aux2) > 0) {
                q_size[i] = aux2;
            } else {
                q_size[i] = aux;
            }
            aux -= aux2;
        } else {
            q_size[i] = 0;
        }
    }

    if (new_size <= min_size) {
        /* If my children use less than l sectors, 
           I'll sort them in memory. Reads will be 
           sequential but limited to new_size sectors.*/
        parity = level % 2;
        fd_in = fd1;
        fd_out = fd2;
        lseek(fd_in, start * sizeof(int), SEEK_SET);
        for (i = 0; i < k; i++) {
            if (q_size[i] > 0) {
                qb_refill_max(bufs[i], fd_in, q_size[i] * sizeof(int));
                m_quicksort(bufs[i]);
            }
        }
    } else {
        for (i = 0; i < k ; i++) {
            mergesort(fd1, fd2, level + 1,  start + i * new_size, q_size[i]);
        }
        fd_in = (parity == (level % 2))? fd1 : fd2;
        fd_out = (parity == (level % 2))? fd2 : fd1;
    }

    /* After this, we have k sorted children of size at most new_size.
       It's merge time! */

    int r = 0;
    /* Only when I have written all integers I will be free :c */

    /* We should fill the buffs up to new_size bytes. We must do
       a seek() before each refill, except in the deepest recursion
       level. In that case, data is already in memory c: */

    if (new_size > min_size)  {
        for (i = 0; i < k ; i++) {
            if (q_size[i] > 0) {
                lseek(fd_in, (start + i * new_size) * sizeof(int), SEEK_SET);
                qb_refill_max(bufs[i], fd_in, q_size[i] * sizeof(int));
                if (!qb_empty(bufs[i])) {
                    pq_insert(heap, qb_dequeue(bufs[i]), i);
                    done[i]++;
                }
            }
        }
    } else {
        for (i = 0; i < k; i++) {
            if (!qb_empty(bufs[i])) {
                pq_insert(heap, qb_dequeue(bufs[i]), i);
                done[i]++;
            }
        }
    }

    while (r < size) { 

        while (!pq_empty(heap)) {
            pq_extract(heap, &value, &qb_idx);
            if (!qb_empty(bufs[qb_idx])) {
                pq_insert(heap, qb_dequeue(bufs[qb_idx]), qb_idx);
                done[qb_idx]++;
            } else {
                if (done[qb_idx] < q_size[qb_idx]) {
                    lseek(fd_in, (start + qb_idx * new_size + done[qb_idx]) * sizeof(int), SEEK_SET);  
                    qb_refill_max(bufs[qb_idx], fd_in, (q_size[qb_idx] - done[qb_idx]) * sizeof(int));
                    if (!qb_empty(bufs[qb_idx])) {
                        pq_insert(heap, qb_dequeue(bufs[qb_idx]), qb_idx);
                        done[qb_idx]++;
                    }
                }
            }

            qb_enqueue(ex, value);
            if (qb_full(ex)) {
                lseek(fd_out, (start + r) * sizeof(int), SEEK_SET);
                written = qb_flush(ex, fd_out);
                r += written/(sizeof(int)) ;
            }

        }
        /* If the priority queue is empty, then we have to flush
           the exit buf, whether it is full or not. */
        if (!qb_empty(ex)) {
            lseek(fd_out, (start + r) * sizeof(int) , SEEK_SET);
            written = qb_flush(ex, fd_out);
            r += written/(sizeof(int));
        }
    }

 //       swap(fd_in, fd_out);
}

void m_quicksort(struct queue_buf *q){//funciona perro :D
    int *buff;
    int size;
    qb_get_array(q, &buff, &size);
    sort(buff, 0, size);
}

void swap(int *a, int *b){
    int t=*a; *a=*b; *b=t;
}

void sort(int *arr, int beg, int end){
    int piv;
    int q;
    int r;
    if(end > beg + 1){
        piv = arr[beg], q = beg + 1, r = end;
        while(q < r){
            if (arr[q] <= piv)
                q++;
            else
                swap(&arr[q], &arr[--r]);
        }
        swap(&arr[--q], &arr[beg]);
        sort(arr, beg, q);
        sort(arr, r, end);
    }
}

