#include "alpha_samplesort.h"

#include "../utils/queue_buf.h"
#include "../utils/priority_queue.h"
#include "../utils/sort_utils.h"
#include "../utils/test_utils.h"

#define ceildiv(a,b) ((a) + (b) - 1 ) / (b);

void m_quicksort(int *buf, off_t size);


void s_samplesort(int fd,int floor, off_t size){
  int k;
  k = ceildiv(size,M);
  if(k < 1){
    perror("this is going bad\n");
    exit(1);
  }
  struct queue_buf *buff;
  int *files, *sizes, *keys;
  int i,j,cur,r,ret,ret1;
  char name_file[50];
  
  r = 0;
  buff = qb_new(M);
  files = (int *)malloc(sizeof(int)*k);
  sizes = (int *)malloc(sizeof(int)*k);
  keys = (int *)malloc(sizeof(int)*(k+1));    
  
  select_keys(keys,fd,size);

  for(i = 0; i < k; i++){
		sizes[i] = 0;
  }
  for(i = 0; i < k ; i++ ){
    sprintf(name_file, "buffer%d_%d\0", floor,i);
    if((files[i] = open(name_file, O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO))== -1){
      printf("buffer%d_%d fail1\n",floor, i);
      perror("aca");
      exit(1);
    }
  }
  while(r < size){
    if((ret1 = qb_refill(buff,fd))== -1){
      printf("buffer%d_%d fail2\n",floor, i);
      perror("aca");
      exit(1);
    }
    for(j = 0; j < M ; j++ ){
      cur = qb_dequeue(buff);
      i = bucket(cur,keys,k-1);
      if((ret =  write(files[i],(void *)&cur,sizeof(int)))== -1){
        printf("buffer%d_%d fail3\n",floor, i);
        perror("aca");
        exit(1);
      }
      sizes[i]++;;
    }
    r+=M;
     
  }

  for(i = 0; i < k; i++)
    close(files[i]);
  for(i = 0; i < k; i++){
    if(sizes[i] == 0){	  
      sprintf(name_file, "buffer%d_%d\0", floor,i);
      remove(name_file);
    }
    else if(sizes[i] <= M){
      if((ret1 = qb_refill(buff,files[i]))== -1){
        printf("buffer%d_%d fail2\n",floor, i);
       perror("aca");
       exit(1);
      }
      quicksort(buff);
      qb_flush(buff,files[i]);
    }
    else{
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
  qb_free(buff);
  free(keys);
  free(files);
  free(sizes);        
}


/*return the bucket corresponding to comp*/
int bucket(int comp, int *key, off_t size){
  int min,max,mid,x;
  min = 0;
  max = size - 1;
  do{
    mid =(min+max)/2;
    if(comp > key[mid])
      min = mid + 1;
    else
      max = mid - 1;
  }
  while((key[mid] != comp) && (min <= max));
  if(key[mid] != comp)
    return mid+1;
  return mid;  
}

/*calculates the keys for the current recursion using samples*/
void select_keys(int *keys, int fd, off_t size){
    int a,k,i,j,random_integer;
    double b;
    int *samples;

    k = size/M;
    b =(log(k)/log(2));
    a = (int) b;
    if(k < 1)
      perror("this is going bad\n");
    samples = (int*)malloc(sizeof(int)*((a+1)*k));
    keys[0] = INT_MIN;
    keys[k] = INT_MAX;
    srand((unsigned)time(0));
    
    for(i=1;i < ((a+1)*k -1);i++){
      random_integer = rand() % (size*sizeof(int)/B);
      lseek(fd, random_integer*B, SEEK_SET);
      if((j = read(fd, (void *)&(samples[i]),sizeof(int)))== -1){
        printf("fail4\n");
        exit(1);
      }
    }

    lseek(fd, 0, SEEK_SET);
    m_quicksort(samples,(a+1)*k); 
    for(i=1;i < k;i++){
      keys[i] = samples[(a+1)*i];
    }
}



//utilities
void m_quicksort(int *buf, off_t size){
  sort(buf, 0, size);
}
