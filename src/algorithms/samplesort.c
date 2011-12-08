#include "samplesort.h"

int samples[m];
int keys[k];
int files[k];
char name_file[50];
int sizes[k];

void fill_samples(int *samples, int fd);
void fill_files();
void sort_files(int fd);

void samplesort(int fd){

	/*hay que hacer esto para todo el archivo grande*/

	int i;

	fill_samples(samples,fd);

	/*limite min*/
	keys[0] = 0;

	/*iniciamos pivotes*/
	for(i = 1; i < k; i++)
		keys[i] = samples[(a + 1)*i];


	/*ordenar pivotes*/
	sort(0,k,keys);


	/*crear archivos*/
	for(i = 0; i < k ; i++ ){
		sizes[i]=0;
		memset(&name_file[0],0,sizeof(name_file));
		sprintf(name_file, "buffer%d\n", i);
		files[i] = open(name_file[i], O_RDWR | O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
	}
	
	/*escribimos pivotes en sus archivos correspondientes*/
	for(i = 1; i < k; i++){
		write(files[i-1], keys[i], sizeof(int));
		sizes[i-1]++;
	}
	
	fill_files();
	
	/*¿sera necesario?*/
	for(i = 0 ; i < k ; i++)
		lseek(files[i], 0, SEEK_SET);
	
	sort_files(fd);

}

void fill_samples(int *samples, int fd){
	
	read(fd, (void *)samples,sizeof(int)*m);
	
}

void fill_files(){
	
	int i,ini,fin,me,valor,temp;
	
	for(i = 0; i < m ; i++){
		
		valor = samples[i];
			
		ini = 0;
		fin = k-1;
		me = (fin - ini)/2;
			
		if(valor > keys[k-1]){
				write(files[k-1], valor, sizeof(int));
				sizes[k-1]++;
				continue;
		}
			
		while(1){
			
			if((fin - ini) == 1){
				write(files[ini], valor, sizeof(int));
				sizes[ini]++;
				break;
			}
			
			if(valor == keys[me]){
				write(files[ini], valor, sizeof(int));
				sizes[me]++;
				break;
			}
		
			if(valor < keys[me]){
				fin = me;
				me = (fin - ini)/2;
			}
			else{
				ini = me;
				me = (fin - ini)/2;
			}
			
		}
		
	}
	
}

void sort_files(int fd){
	
	/*hacer quickaux de un arreglo de elementos del archivo*/
	int i ;
	for(i = 0; i < k; i++){		
		int bucket[sizes[i]];
		read(files[i], (void *)bucket,sizeof(int)*sizes[i]);
		sort(bucket,0,sizes[i]);
		/*se podria escribir altiro en el archivo final*/
	}
}
