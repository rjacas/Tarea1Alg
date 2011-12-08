#include "m_mergesort.h"

#include "../utils/queue_buf.h"
#include "../utils/priority_queue.h"

struct queue_buf* bufs[k]; 
struct queue_buf* ex;
struct priority_queue* heap;

void m_mergesort(int fd){
  int i;
  heap = pq_new(k);
  for( i = 0; i < k; i++){
    bufs[i] = qb_new(l);
  }
  ex = qb_new(l);  
  mergesort(fd, n, 0);
  pq_free(heap);
} 

void mergesort(int fd, int size, int pos){
  if(size <= l)
    m_quicksort(bufs[pos]);
  else{
    int i;
    for(i = 0;i < k; i++){
      mergesort(fd,size/k,i);
    }
  }
  int r;
  while(r < size){
    int j;
    for(j = 0; j < k; j++){
      qb_refill(bufs[j],fd);
      pq_insert(heap, qb_dequeue(bufs[j]), j);
    }
    while(!pq_empty(heap)){
        int x;
        int y;
        pq_extract(heap,&x, &y);
        if(!qb_empty(bufs[y]) && ((heap->elems[0].i1 + 1) != heap->elems[0].i2)){
          pq_insert(heap, qb_dequeue(bufs[y]), y);
        }
        qb_enqueue(ex,x);
        if(qb_full(ex)){
          qb_flush(ex, fd);
        }
    }
    r += k;
  }
  if(pos == k)
    lseek(fd, 0, SEEK_SET);
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

