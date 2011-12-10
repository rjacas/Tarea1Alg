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
/*		while(read(files[i].file,&valor,sizeof(int))>0)
			printf("%d\n",valor);		
		printf("\n");	*/
	}


}

/*calculates the keys for the current recursion using samples*/
void select_keys(int *keys, int fd,int pos, int max){
		int p=a;
    int i,j,random_integer;
    int samples[(p+1)*k];
    srand((unsigned)time(0));    
    for(i=1;i < ((p+1)*k -1);i++){
      random_integer = rand() % (N*sizeof(int)/B);
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
		kfiles[c].size=0;
		for(j = 0; j < narchivos; j++){
			memset(&kfiles[c].name[0],0,sizeof(&kfiles[c].name[0]));
			sprintf(&kfiles[c].name[0], "%s%d",prefiles[i].name ,j+1);
			kfiles[c].file = open(&kfiles[n].name[0], O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
			c++;
		}
	}
		
	for(i = 0; i < narchivos; i++){
		if(prefiles[i].size==0)
			continue;
		
		select_keys(kkeys,prefiles[i].file,(k*i)+1,prefiles[i].max);
		/*printf("llenando subarchivos desde %d hasta %d\n",i*k,(i+1)*k-1);*/
		fill_files(prefiles[i].file,i*k,(i+1)*k-1,kfiles,kkeys);
		/*printf("subarchivos llenados\n");*/
		close(prefiles[i].file);
		remove(prefiles[i].name);
		kfiles[i].nivel = nivel;
	}

	/*for(i = 0; i < (k*narchivos)+1; i++)
		printf("key %d vale %d\n",i,kkeys[i]);	*/
		
	
	fill = fill_files(filefd,0,k*narchivos,kfiles,kkeys);
	
	if(fill)
		sort_files(kfiles,k*narchivos);
	
	for(i = 0 ; i < k*narchivos ; i++){
		close(kfiles[i].file);
		if(kfiles[i].size==0){
			remove(kfiles[i].name);
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

int fill_files(int fd, int inicio, int final, struct bucket *archivos,int *keys){
	
	int i,ini,fin,me,valor;
	
	while(read(fd,&valor,sizeof(int))>0){
		/*printf("valor= %d\n",valor);		*/
		ini = inicio;
		fin = final;
		me = (fin + ini)/2;
		
		while(1){
			if((fin - ini) == 1){
				write(archivos[ini].file, (void *)&valor, sizeof(int));
				archivos[ini].size++;
				/*printf("escribiendo en archivo %d\n",ini);		*/
				break;
			}
			
			if(valor == keys[me]){
				write(archivos[me-1].file, (void *)&valor, sizeof(int));
				archivos[me-1].size++;
				/*printf("escribiendo en archivo %d\n",me-1);		*/
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
	select_keys(keys,filefd,1,INT_MAX);

	/*crear archivos*/
	for(i = 0; i < k ; i++ ){
		files[i].size = 0;
		memset(&files[i].name[0],0,sizeof(&files[i].name));
		sprintf(files[i].name, "resultados/buffer%d", i+1);
		files[i].file = open(files[i].name, O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
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
		if(files[i].size==0){
			remove(files[i].name);
		}
	}

}

int main(){
	
	int fd;

	fd = open("test_bucket2", O_RDWR);
	
	samplesort(fd);
	
}
