#include "m_mergesort.h"

#include "../utils/queue_buf.h"
#include "../utils/priority_queue.h"

#define ceildiv(a,b) ((a) + (b) - 1 ) / (b);

int B = 128;
int M = 26214400;
int m= 26214400 / 128;
int l; // = m / k = length of buffers, in blocks
off_t min_size; //l * B;
int k;
struct queue_buf **bufs; 
struct queue_buf *ex;
struct priority_queue *heap;
int parity;
struct sort_results results;


struct sort_results m_mergesort(int fd1, int fd2, int user_k, off_t N){
    int i, aux;
    results.io_acc = results.io_rand = 0;
    k = user_k;
    heap = pq_new(k);
    l = m/k; 
    min_size = l * B;
    bufs = (struct queue_buf **) malloc(k * sizeof(struct queue_buf *));
    for (i = 0; i < k; i++){
        bufs[i] = qb_new(l * B);
    }

    ex = qb_new(l * B);  
    mergesort(fd1, fd2, 0, 0, N);
    pq_free(heap);
    for (i = 0; i < k; i++) {
        qb_free(bufs[i]);
    }

    free(bufs);
    return results;
} 

void mergesort(int fd1, int fd2, int level, off_t start, off_t size) {
    int i;
    off_t done[k];
    off_t q_size[k];
    off_t new_size; 
    off_t written, aux, aux2;
    int value;
    int qb_idx;
    int fd_in, fd_out;
    off_t r;
    /* This is done only because this would be the only use
       of math.h . If you end up including math.h, change this
       to ceil(size/k) */
    new_size = ceildiv(size, k);
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

    r = 0;
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
                results.io_acc += ceildiv(bufs[i]->n_elems, B * sizeof(int));
                results.io_rand++;
                quicksort(bufs[i]);
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

    /* Only when I have written all integers I will be free :c */

    /* We should fill the buffs up to new_size bytes. We must do
       a seek() before each refill, except in the deepest recursion
       level. In that case, data is already in memory c: */

    if (new_size > min_size)  {
        for (i = 0; i < k ; i++) {
            if (q_size[i] > 0) {
                lseek(fd_in, (start + i * new_size) * sizeof(int), SEEK_SET);
                qb_refill_max(bufs[i], fd_in, q_size[i] * sizeof(int));
                
                results.io_acc += ceildiv(bufs[i]->n_elems, B * sizeof(int));
                results.io_rand++;

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
                    results.io_acc += ceildiv(bufs[qb_idx]->n_elems, B * sizeof(int));
                    results.io_rand ++;
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
                results.io_acc += ceildiv(written, B * sizeof(int));
                results.io_rand ++;
            }

        }
        /* If the priority queue is empty, then we have to flush
           the exit buf, whether it is full or not. */
        if (!qb_empty(ex)) {
            lseek(fd_out, (start + r) * sizeof(int) , SEEK_SET);
            written = qb_flush(ex, fd_out);
            r += written/(sizeof(int));
            results.io_acc += ceildiv(written, B * sizeof(int));
            results.io_rand ++;
        }
    }

}

