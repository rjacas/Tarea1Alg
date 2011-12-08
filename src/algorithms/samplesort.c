#include "samplesort.h"

int samples[m];
int keys[k];
int files[k];
char name_file[50];
/* podria haber un arreglo para guardar tamano 
 * final de cada archivo, asi es mas facil ordenar*/

void samplesort(int fd){

	int i;

	fill_samples(samples,fd);

	keys[0] = 0;

	for(i = 1; i < k; i++ ){
		
		keys[i] = samples[(a + 1)*i];
		
	}

	quickaux(0,k-2,keys);

	for(i = 0; i < k ; i++ ){
		memset(&name_file[0],0,sizeof(name_file));
		sprintf(name_file, "buffer%d\n", i);
		files[i] = open(name_file[i], O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
	}
	
	fill_files();
	
	/*¿sera necesario?*/
	for( i = 0 ; i < k ; i++ )
		close(files[i]);
	
	sort_files();

}

void fill_samples(int *samples, int fd){
	
	read(fd, (void *)samples,sizeof(int)*m);
	
}

void fill_files(){
	
	int i,ini,fin,me,valor,temp;
	
	for(i = 0; i < m ; i++ ){
		
		valor = samples[i];
			
		ini = 0;
		fin = k-1;
		me = (fin - ini)/2;
			
		if(valor > keys[k-1]){
				write(files[k-1], valor, sizeof(int));
				continue;
		}
			
		while(1){
			
			if(((fin - ini) == 1) || (valor == keys[me])){
				write(files[ini], valor, sizeof(int));
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

void sort_files(){
	
	quickaux();

}
