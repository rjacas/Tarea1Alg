#include "alpha_samplesort.h"

#include "../utils/queue_buf.h"
#include "../utils/priority_queue.h"
#include "../utils/test_utils.h"

#define ceildiv(a,b) ((a) + (b) - 1 ) / (b)
#define min(a,b) ((a) - (b) < 1 ) ? (a) : (b)
#define max(a,b) ((a) - (b) < 1 ) ? (b) : (a)


struct sort_results results;


struct sort_results alpha_samplesort(int fd, off_t size,char *base_name,int k){
    
    results.io_acc = 1;
    results.io_rand = 0;
    s_samplesort(fd, size, base_name,k);
    return results;
} 

void s_samplesort(int fd, off_t size,char *base_name,int k){
  //int k;
  //k = min(ceildiv(size,M),ceildiv(M,B));

  if(k < 1){
    perror("this is going bad\n");
    exit(1);
  }
  struct queue_buf *buff;
  struct queue_buf **file_buff;
  int *files, *sizes, *keys;
  int i,j,cur,r,ret,ret1;
  char name_file[500];
  
  r = 0;
  buff = qb_new(M);
  files = (int *)malloc(sizeof(int)*k);
  sizes = (int *)malloc(sizeof(int)*k);
  keys = (int *)malloc(sizeof(int)*(k+1));    
  file_buff = (struct queue_buf **)malloc(sizeof(struct queue_buf *)*k);
  select_keys(keys,fd,size,k);
  

  for(i = 0; i < k; i++){
    file_buff[i] = qb_new(B);
		sizes[i] = 0;
    sprintf(name_file, "%s%d\0", base_name,i);
    if((files[i] = open(name_file, O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO))== -1){
      printf("%s%d fail1\n", base_name,i);
      perror("aca");
      exit(1);
    }
    results.io_rand++;
  }
  j=1;
  while(1){
    if((ret1 = qb_refill(buff,fd))== -1){
      printf("%s%d fail2\n", base_name,i);
      perror("aca");
      exit(1);
    }
    results.io_rand++;
    results.io_acc+=ceildiv(buff->n_elems,B);
    
    if (qb_empty(buff)) break;
    
    while(!qb_empty(buff)){
      cur = qb_dequeue(buff);
      i = bucket(cur,keys,k+1);
      qb_enqueue(file_buff[i], cur);
      if(qb_full(file_buff[i])){
        results.io_rand++;
        results.io_acc+=ceildiv(file_buff[i]->n_elems,B);
      	sizes[i] += file_buff[i]->n_elems;
        if((ret = qb_flush(file_buff[i],files[i]))== -1){
          printf("%s%d fail3\n", base_name,i);
          perror("aca");
          exit(1);
        }
      }
    }     
  }
  
  /* Pueden quedar restos en los bufs. Hay que vaciar todo */
  for (i = 0; i < k; i++) {
  	if (!qb_empty(file_buff[i])) {
        results.io_rand++;
        results.io_acc+=ceildiv(file_buff[i]->n_elems,B);
  	    sizes[i] += file_buff[i]->n_elems;
  	    qb_flush(file_buff[i], files[i]);
  	}
  	close(files[i]);
  	qb_free(file_buff[i]);
  }
  
  free(file_buff);
      
  free(keys);
  
  for(i = 0; i < k; i++){
		sprintf(name_file, "%s%d\0", base_name,i);
    if(sizes[i] == 0){	  
      remove(name_file);
    }
    else if(sizes[i] <= M){
      if((files[i] = open(name_file, O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO))== -1){
        printf("%s%d fail1\n", base_name,i);
        perror("aca");
        exit(1);
      } 
      if((ret1 = qb_refill(buff,files[i]))== -1){
        printf("%s%d fail2\n", base_name,i);
				perror("aca");
				exit(1);
      }
      results.io_rand++;
      results.io_acc+=ceildiv(buff->n_elems,B);
      quicksort(buff);
      lseek(files[i],0,SEEK_SET);
      results.io_rand++;
      qb_flush(buff,files[i]);
      results.io_rand++;
      results.io_acc+=ceildiv(buff->n_elems,B);
      close(files[i]);
    }
  }
  
  qb_free(buff);
  
  for (i = 0; i < k; i++) {
		sprintf(name_file, "%s%d\0", base_name,i);
    if (sizes[i] > M) {
      if((files[i] = open(name_file, O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO))== -1){
        printf("%s%d fail1\n",base_name, i);
        perror("aca");
        exit(1);
      }
      results.io_rand++;    
      s_samplesort(files[i],sizes[i],name_file,k);
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


void select_keys(int *keys, int fd, off_t size, int k){
    int a,i,j,random1,random2;
    off_t le_random,random_integer;
    int *samples;
    a = ceildiv(ceil(log(k)),ceil(log(2)));

    samples = (int*)malloc(sizeof(int)*((a+1)*k));
    keys[0] = INT_MIN;
    keys[k] = INT_MAX;
    srand((unsigned)time(0));
      
    for(i=0;i < (a+1)*k ;i++){
        random1 = rand(); 
        random2 = rand();
        le_random = random1;
        le_random = le_random << 32 | random2;
        random_integer = le_random % (size/B);
				lseek(fd, sizeof(int)*random_integer, SEEK_SET);
				if((j = read(fd, (void *)&(samples[i]),sizeof(int)))== -1){
					printf("fail4\n");
					exit(1);
				}
        results.io_acc++;
        results.io_rand++;
    }

    lseek(fd, 0, SEEK_SET);
    results.io_rand++;
    alpha_quicksort(samples,(a+1)*k); 
    for(i=1;i < k;i++){
      keys[i] = samples[(a+1)*i];
    }
    free(samples);
}
