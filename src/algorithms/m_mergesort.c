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
    m_quicksort(fd,size,pos);
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
        if(!qb_empty(bufs[y]))
          pq_insert(heap, qb_dequeue(bufs[y]), y);
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

void m_quicksort(int fd, int size, int pos){
  int ret;
  int* buff; 
  buff = (int *)malloc(sizeof(int) * size);
  ret = read(fd, (void *)buff, size);
  lseek(fd, pos, SEEK_SET);
  sort(buff, 0, size);
  write(fd, (void *)buff, size);
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

int main(){
    int fd, i;
    int foo[N_ELEMS];
    struct queue_buf *qb;
    fd = open("test_file", O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
    for (i = 0; i < N_ELEMS; i++) {
        foo[i] = rand();
        printf("Foo[%d]=%d\n", i, foo[i]);
    }
    write(fd, (void *)foo, sizeof(int)*N_ELEMS);
    m_quicksort(fd, N_ELEMS,0);
    close(fd);
    fd = open("test_file", O_RDWR);
    qb = qb_new(N_ELEMS);
    qb_refill(qb, fd);
    for (i = 0; i < N_ELEMS; i++) {
        printf("Extracted %d\n", qb_dequeue(qb));
    }
    return 1;
}

