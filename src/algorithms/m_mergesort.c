#include "m_mergesort.h"

#include "../utils/queue_buf.h"
#include "../utils/priority_queue.h"

int B = 2;
int N = 24;
int M = 24;
int m = 12; //  M/B
int k = 3;  //
int l = 4; // = m / k 
int n = 12; //N/B

//replace k with 3
struct queue_buf *bufs[3]; 
struct queue_buf *ex;
struct priority_queue *heap;

void m_mergesort(int fd){
  int i, aux;
  heap = pq_new(k);
  for( i = 0; i < k; i++){
    bufs[i] = qb_new(l * B);
  }
  ex = qb_new(l * B);  
  mergesort(fd, n, 0, 0);
  pq_free(heap);
} 

void mergesort(int fd, int size, int file_pos) {
  int i, new_size;
 
  int value;
  int qb_idx;
  /* This is done only because this would be the only use
     of math.h . If you end up including math.h, change this
     to ceil(size/k) */
  new_size = ( size + k - 1 ) / k;

  if (new_size <= l) {
      /* If my children use less than l sectors, 
         I'll sort them in memory. Reads will be 
         sequential but limited to new_size sectors.*/
      lseek(fd, file_pos, SEEK_SET);

      for (i = 0; i < k; i++) {
        qb_refill_max(bufs[i], fd, new_size * B);
        m_quicksort(bufs[i]);
      }
  } else {
      for (i = 0; i < k; i++) {
        mergesort(fd, new_size, file_pos + i * B * sizeof(int) * new_size);
      }
  }

  /* After this, we have k sorted children of size new_size.
     It's merge time! */

  int r = 0;
  /* Only when I have merged all sectors I will be free :c */
  while (r < size) {

    /* We should fill the buffs up to new_size bytes. We must do
       a seek() before each refill, except in the deepest recursion
       level. In that case, data is already in memory c: */

    if (new_size > l)  {
        for (i = 0; i < k; i++) {
            lseek(fd, file_pos + i * new_size * B * sizeof(int), SEEK_SET);
            qb_refill(bufs[i],fd);
            pq_insert(heap, qb_dequeue(bufs[i]), i);
        }
    } else {
        for (i = 0; i < k; i++) {
            pq_insert(heap, qb_dequeue(bufs[i]), i);
        }
    }
   
    if (r == 0) {
        printf("first\n");
    }
    lseek(fd, file_pos + r * B * sizeof(int) , SEEK_SET);

    while (!pq_empty(heap) && r < size) {
        pq_extract(heap, &value, &qb_idx);
        i++;
        if (!qb_empty(bufs[qb_idx])) {
          pq_insert(heap, qb_dequeue(bufs[qb_idx]), qb_idx);
        printf("Got %d from %d\n", value, qb_idx); 
        } else {
            printf("foo\n");
        }
        
        qb_enqueue(ex, value);
        if (qb_full(ex)) {
            qb_flush(ex, fd);
            r += l;
        }
    }
    /* If the priority queue is empty, then we have to flush
       the empty buf, whether it is full or not. */
    if (!qb_empty(ex)) {
        qb_flush(ex, fd);
        r += l;
    }

  }

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

