#include "m_mergesort.h"

queue_buf* bufs[k]; 
queue_buf * exit;
priority_queue* heap;


void m_mergesort(int in){
  int i;
  heap = pq_new(k);
  for( i = 0; i < k; i++){
    bufs[i] = queue_buf_new(l);
  }
  exit = queue_buf_new(l);   
} 

void mergesort(int fd, int size, int pos){
  if(size <= l)
    quicksort(..);//hacer
  else{
    int i;
    for(i = 0;i < k; i++){
      mergesort(fd,size/k,i);
    }
  }
  //masta while
  int j;
  for(j = 0; j < k; j++){
    refill_queue_buf(bufs[j],fd);
    pq_insert(heap, dequeue(bufs[j]), j);
  }
  while(!empty(heap)){
      int x;
      int y;
      pq_extract(heap,&x, &y);
      if(!empty(buf[y])
        pq_insert(heap, dequeue(bufs[y]), y);
      enqueue(exit,x);
      if(full(exit))
        flush(..);//hacer
  }  
  
}
