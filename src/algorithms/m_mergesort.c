#include "m_mergesort.h"

queue_buf* bufs[k]; 
queue_buf* exit;
priority_queue* heap;

void m_mergesort(int in){
  int i;
  heap = pq_new(k);
  for( i = 0; i < k; i++){
    bufs[i] = queue_buf_new(l);
  }
  exit = queue_buf_new(l);  
  mergesort(fd, n, 0);
} 

void mergesort(int fd, int size, int pos){
  if(size <= l)
    quicksort(fd,size);
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
      refill_queue_buf(bufs[j],fd);
      pq_insert(heap, dequeue(bufs[j]), j);
    }
    while(!empty(heap)){
        int x;
        int y;
        pq_extract(heap,&x, &y);
        if(!empty(buf[y])
          pq_insert(heap, dequeue(bufs[y]), y);
        enqueue(exit,x);//cosas pasan
        if(full(exit)){
          write(fd, (void*) exit->elems, l);//falta vaciar 
        }
    }
    r += k;
  }
  if(pos == k)
    lseek(fd, 0, SEEK_SET);
}

void quicksort(int fd, int size){
  int ret;
  int* buff; 
  buff = (int *)malloc(sizeof(int) * size);
  ret = read(fd, (void *)buff, size);
  quickaux(0, size - 1, buff);
  write(fd, (void *)buff, size);
  free(buff);
}

void quickaux(int begin, int end,int *buf){
  int i,j,pivot;
  pivot = buf[end];
  i = begin;
  j = end;
  while(i != j){
    if (buf[i] < pivot) { 
      i += 1; 
    } 
    else { 
      buf[j] = buf[i]; 
      buf[i] = buf[j-1]; 
      j -= 1; 
    }     
  }
  buf[j] = pivot;
  quickaux(start, j-1,buf);
  quickaux(j+1, end,buf); 
} 
