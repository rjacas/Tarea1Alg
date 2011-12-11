#include "alpha_samplesort.h"

int buff[M];



int bucket(int comp, int *key, off_t size);
void select_keys(int *keys, int fd,off_t size);
void s_samplesort(int fd,int floor,off_t size);

void print_file_integers(int fd, off_t size);

void m_quicksort(int *buff,off_t size);
void swap(int *first, int *second);
void sort(int *arr, int beg, int end);


int main(int argc, char **argv){
  int ret,ret1;
  int fd;
  int i,j;
  int N;
   
  if(argc < 3){
    printf("need 2 arguments\n");
    exit(1);
  }
  N = atoi(argv[2]);
  if((fd = open(argv[1], O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO))== -1)
      perror("fail");

  s_samplesort(fd,0,N);

  print_file_integers(fd, N*4);
  close(fd);
  return 0;
}


void s_samplesort(int fd,int floor, off_t size){
  int k;
  k = size/M;
  if(k < 1){
    perror("this is going bad\n");
    exit(1);
  }
  int files[k];
  int sizes[k];
  int keys[k-1];
  int i,j,r,ret,ret1;
  char name_file[50];
  r = 0;

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
    if((ret1 = read(fd, (void *)buff,sizeof(int)*M))== -1){
      printf("buffer%d_%d fail2\n",floor, i);
      perror("aca");
      exit(1);
    }
    for(j = 0; j < M ; j++ ){ 
      i = bucket(buff[j],keys,k-1);
      if((ret = write(files[i],(void*)&(buff[j]),sizeof(int)))== -1){
        printf("buffer%d_%d fail3\n",floor, i);
        perror("aca");
        exit(1);
      }
      sizes[i]++;;
    }
    r+=M;
     
  }
  for(i = 0; i < k; i++){
    if(sizes[i] == 0){	  
      close(files[i]);
      sprintf(name_file, "buffer%d_%d\0", floor,i);
      remove(name_file);
    }
    else if(sizes[i] <= M)
      close(files[i]);
    else{
      lseek(files[i], 0, SEEK_SET);
      s_samplesort(files[i],floor+1,sizes[i]);
      close(files[i]);
      sprintf(name_file, "buffer%d_%d\0", floor,i);
      remove(name_file);
    }
  }      
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
    int a,k;
    double b;
    k = size/M;
    b =(log(k)/log(2));
    a = (int) b;
    if(k < 1)
      perror("this is going bad\n");
    int i,j,random_integer;
    int samples[(a+1)*k];
    keys[0] = 0;
    keys[k-1] = INT_MAX;
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
    for(i=1;i < (k - 1);i++){
      keys[i] = samples[(a+1)*i];
    }
}



//utilities
void print_file_integers(int fd, off_t size){
    int ret, i;
    int buf[size];
    lseek(fd, 0,SEEK_SET);

    while ((ret = read(fd, (void *)buf, size * sizeof(int))) > 0) {
        for (i = 0; i < ret/sizeof(int); i++) {
            printf ("%d,%d Read integer: %d\n",fd,i, buf[i]);
        }
    }
}

void m_quicksort(int *buff, off_t size){
  sort(buff, 0, size);
}

void swap(int *first, int *second){
  int t;
  t=*first; *first=*second; *second=t;
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
