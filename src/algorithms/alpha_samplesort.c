#include "alpha_samplesort.h"

#include "../utils/queue_buf.h"
#include "../utils/priority_queue.h"
#include "../utils/sort_utils.h"
#include "../utils/test_utils.h"

#define ceildiv(a,b) ((a) + (b) - 1 ) / (b);


void s_samplesort(int fd,int floor, off_t size){
  int k;
  k = ceildiv(size,M);
  printf("nivel %d k %d\n",floor,k);
  if(k < 1){
    perror("this is going bad\n");
    exit(1);
  }
  struct queue_buf *buff;
  struct queue_buf **file_buff;
  int *files, *sizes, *keys;
  int i,j,cur,r,ret,ret1;
  char name_file[50];
  
  r = 0;
  buff = qb_new(M);
  files = (int *)malloc(sizeof(int)*k);
  sizes = (int *)malloc(sizeof(int)*k);
  keys = (int *)malloc(sizeof(int)*(k+1));    
  file_buff = (struct queue_buf **)malloc(sizeof(struct queue_buf *)*k);
  select_keys(keys,fd,size);
  

  for(i = 0; i < k; i++){
    file_buff[i] = qb_new(B);
		sizes[i] = 0;
    sprintf(name_file, "buffer%d_%d\0", floor,i);
    if((files[i] = open(name_file, O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO))== -1){
      printf("buffer%d_%d fail1\n",floor, i);
      perror("aca");
      exit(1);
    }
  }
  j=1;
  while(1){
    if((ret1 = qb_refill(buff,fd))== -1){
      printf("buffer%d_%d fail2\n",floor, i);
      perror("aca");
      exit(1);
    }
    
    if (qb_empty(buff)) break;
    
    while(!qb_empty(buff)){
      cur = qb_dequeue(buff);
      i = bucket(cur,keys,k+1);
      qb_enqueue(file_buff[i], cur);
      if(qb_full(file_buff[i])){
      	sizes[i] += file_buff[i]->n_elems;
        if((ret = qb_flush(file_buff[i],files[i]))== -1){
          printf("buffer%d_%d fail3\n",floor, i);
          perror("aca");
          exit(1);
        }
      }
    }     
  }
  
  /* Pueden quedar restos en los bufs. Hay que vaciar todo */
  for (i = 0; i < k; i++) {
  	if (!qb_empty(file_buff[i])) {
  	    sizes[i] += file_buff[i]->n_elems;
  	    qb_flush(file_buff[i], files[i]);
  	}
  	close(files[i]);
  	qb_free(file_buff[i]);
  }
  
  free(file_buff);
      
  free(keys);
  
  for(i = 0; i < k; i++){
    if(sizes[i] == 0){	  
      sprintf(name_file, "buffer%d_%d\0", floor,i);
      remove(name_file);
    }
    else if(sizes[i] <= M){
      if((files[i] = open(name_file, O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO))== -1){
        printf("buffer%d_%d fail1\n",floor, i);
        perror("aca");
        exit(1);
      } 
      printf("name %s size %d\n",name_file,sizes[i]);
      if((ret1 = qb_refill(buff,files[i]))== -1){
        printf("buffer%d_%d fail2\n",floor, i);
       perror("aca");
       exit(1);
      }
      quicksort(buff);
      qb_flush(buff,files[i]);
      close(files[i]);
    }
  }
  
  qb_free(buff);
  
  for (i = 0; i < k; i++) {
    if (sizes[i] > M) {
      sprintf(name_file, "buffer%d_%d\0", floor,i);
      if((files[i] = open(name_file, O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO))== -1){
        printf("buffer%d_%d fail1\n",floor, i);
        perror("aca");
        exit(1);
      }      
      s_samplesort(files[i],floor+1,sizes[i]);
      sprintf(name_file, "buffer%d_%d\0", floor,i);
      close(files[i]);
      remove(name_file);
    }
  }
  free(sizes);   
  free(files);
}



int bucket(int comp, int *key, off_t size){
  int min,max,mid,x;
  min = 0;
  max = size - 1;
  do{
    mid =(min+max)/2;
    if((max-min)==1)
			return min;
    if(comp > key[mid])
      min = mid;
    else
      max = mid;
  }
  while((key[mid] != comp) && (min <= max));
  if(key[mid] != comp)
    return mid+1;
  return mid;  
}


void select_keys(int *keys, int fd, off_t size){
    int a,k,i,j,random_integer;
    double b;
    int *samples;

    k = ceildiv(size,M);
    b =ceildiv(log(k),log(2));
    a = (int) b;
    if(k < 1)
      perror("this is going bad\n");
    samples = (int*)malloc(sizeof(int)*((a+1)*k));
    keys[0] = INT_MIN;
    keys[k] = INT_MAX;
    srand((unsigned)time(0));
    
    for(i=0;i < ((a+1)*k -1);i++){
				random_integer = rand() % (size*sizeof(int)/B);
				lseek(fd, random_integer*B, SEEK_SET);
				if((j = read(fd, (void *)&(samples[i]),sizeof(int)))== -1){
					printf("fail4\n");
					exit(1);
				}
    }

    lseek(fd, 0, SEEK_SET);
    alpha_quicksort(samples,(a+1)*k); 
    for(i=1;i < k;i++){
      keys[i] = samples[(a+1)*i];
    }
    free(samples);
}
