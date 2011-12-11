#include "samplesort.h"

int filefd;

int fill_files(int fd, int inicio, int final, struct bucket *archivos,int *keys);

void swap(int *first, int *second){
  int t;
  t=*first; 
  *first=*second; 
  *second=t;
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

void sort_file(int fd,int *nums, int size){
	int i;
	for(i = 0; i< size;i++)
		read(fd,(void *)&nums[i],sizeof(int));
	sort(nums,0,size);
	lseek(fd, 0, SEEK_SET);
	for(i = 0; i< size;i++)
		write(fd,(void *)&nums[i],sizeof(int));
	
}

void sort_files(struct bucket *files, int narchivos){
	int i,valor;

	for(i = 0; i < narchivos; i++){
		if(files[i].size==0)
			continue;
		int nums[files[i].size];
		lseek(files[i].file, 0, SEEK_SET);
		sort_file(files[i].file,nums,files[i].size);
		lseek(files[i].file, 0, SEEK_SET);
		printf("size %d\n",files[i].size);
		/*while(read(files[i].file,&valor,sizeof(int))>0)
			printf("%d\n",valor);*/
		printf("\n");
	}


}

/*calculates the keys for the current recursion using samples*/
void select_keys(int *keys, int fd,int pos, int max,int size){
		int p=a;
    int i,j,random_integer;
    int samples[(p+1)*k];
    srand((unsigned)time(0));    
    int valor;
    lseek(fd, 0, SEEK_SET);
    while(read(fd,&valor,sizeof(int))>0)
			printf("%d\n",valor);
    
    for(i=1;i < ((p+1)*k -1);i++){
      random_integer = rand() % (size*sizeof(int)/B);
      lseek(fd, random_integer*B, SEEK_SET);
      if((j = read(fd, (void *)&(samples[i]),sizeof(int)))== -1)
        perror("fail");
    }
    lseek(fd, 0, SEEK_SET);
    /*for(i=0;i<(p+1)*k;i++)
			printf("samples %d vale %d\n",i,samples[i]);*/
    sort(samples,0,(p+1)*k); 
    for(i=1,j=pos;i < k ;i++){
      keys[j++] = samples[(p+1)*i];
    }
    
    keys[j] = max;
}

void newKfiles(int narchivos,struct bucket *prefiles){

	struct bucket kfiles[k*narchivos];
	int kkeys[(k*narchivos)+1];
	kkeys[0] = INT_MIN;
	int i,j,c = 0, fill = 0;
	int nivel = prefiles[0].nivel+1;

	for(i = 0; i < narchivos ; i++){
		close(prefiles[i].file);
		for(j = 0; j < k; j++){
			kfiles[c].size=0;
			memset(&kfiles[c].name[0],0,sizeof(&kfiles[c].name[0]));
			sprintf(kfiles[c].name, "%s%d",prefiles[i].name ,j+1);
			kfiles[c].file = open(kfiles[n].name, O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
			/*if(kfiles[c].file==-1){
				perror("fd < 0");
				exit(-1);
			}*/
			kfiles[c].nivel = nivel;
			c++;
		}
	}
		int fd,valor;
	for(i = 0; i < narchivos; i++){
		/*if(prefiles[i].size==0)
			continue;*/
		
		fd = open(prefiles[i].name,O_RDWR);
		select_keys(kkeys,fd,(k*i)+1,prefiles[i].max,prefiles[i].size);
		/*while(read(fd,&valor,sizeof(int))>0)
			printf("%d\n",valor);*/
		printf("archivo %s vaciando\n",prefiles[i].name);
		lseek(prefiles[i].file, 0, SEEK_SET);
		fill=fill_files(prefiles[i].file,i*k,(i+1)*k,kfiles,kkeys);
		close(prefiles[i].file);
		//remove(prefiles[i].name);
		
	}

	for(i = 0; i < (k*narchivos)+1; i++)
		printf("key %d vale %d\n",i,kkeys[i]);
		
	fill = fill_files(filefd,0,k*narchivos,kfiles,kkeys);
	
	if(!fill)
		sort_files(kfiles,k*narchivos);
//	int valor;
	for(i = 0 ; i < k*narchivos ; i++){
		lseek(kfiles[i].file, 0, SEEK_SET);
		/*while(read(kfiles[i].file,&valor,sizeof(int))>0)
			printf("%d\n",valor);*/
		printf("\n");
		close(kfiles[i].file);
		printf("file %s size %d\n",kfiles[i].name,kfiles[i].size);
		if(kfiles[i].size==0){
			//remove(kfiles[i].name);
		}
	}

	
}

int ifsuperam(struct bucket *files){
	
	int i;
	
	for(i = 0; i < k; i++){
		if(!(files[i].size > 4*M))
			return 0;
	}
		
	return 1;
	
}

int bucket(int comp, int *key,int mini, int maxi){
  int min,max,mid,x;
  min = mini;
  max = maxi;
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

int fill_files(int fd, int inicio, int final, struct bucket *archivos,int *keys){
	
	int i,ini,fin,me,valor;
	while(read(fd,&valor,sizeof(int))>0){
		ini = inicio;
		fin = final;
		me = (fin + ini)/2;
		
		while(1){
			if((fin - ini) == 1){
				write(archivos[ini].file, (void *)&valor, sizeof(int));
				archivos[ini].size++;
				break;
			}
			
			if(valor == keys[me]){
				write(archivos[me-1].file, (void *)&valor, sizeof(int));
				archivos[me-1].size++;
				//if(fd!=filefd)
			    //printf("escribiendo en archivo %s\n",archivos[me-1].name);	
				break;
			}
		
			if(valor < keys[me]){
				fin = me;
				me = (fin + ini)/2;
			}
			else{
				ini = me;
				me = (fin + ini)/2;
			}
			
		}
		
		if(ifsuperam(archivos)){
			/*printf("nos pasamos\n");*/
			int j,narchivos=1;
			for(j = 0; j < k; j++){
				archivos[j].min = keys[j];
				archivos[j].max = keys[j+1];
			}
			for(j = 0; j < archivos[0].nivel;j++)
				narchivos*=k;
			newKfiles(k,archivos);
			return 1;
		}
		
	}
	return 0;
}

void samplesort(int fd){
	
	filefd=fd;
	
	char name_file[50];
	int keys[k+1],fill=0;
	struct bucket files[k];
	
	int i;

	keys[0]=INT_MIN;	
	/*despues hay que cambiar cambiar ese 1000 por N*/
	select_keys(keys,filefd,1,INT_MAX,1000);

	/*crear archivos*/
	for(i = 0; i < k ; i++ ){
		files[i].size = 0;
		memset(&files[i].name[0],0,sizeof(&files[i].name));
		sprintf(files[i].name, "buffer%d", i+1);
		files[i].file = open(files[i].name, O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
		/*if(files[i].file==-1){
			perror("fd < 0");
			printf("name %s",files[i].name);
			exit(-1);
		}*/
		files[i].nivel = 1;
	}
	
		
	/*aqui hay que empezar a meter los N numeros*/	
	
	/*for(i = 0 ; i < k+1 ; i++)
		printf("key %d vale %d\n",i,keys[i]);*/
		
	fill = fill_files(filefd,0,k,files,keys);
	
	if(!fill)
		sort_files(files,k);
	
	for(i = 0 ; i < k ; i++){
		close(files[i].file);
		printf("file %s size %d\n",files[i].name,files[i].size);
		if(files[i].size==0){
			//remove(files[i].name);
		}
	}

}

int main(){
	
	int fd;

	fd = open("test_bucket1", O_RDWR);
	
	samplesort(fd);
	
}
