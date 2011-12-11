#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>


//N 128
#define M 32
#define B 2




void s_samplesort(int fd,int floor,off_t size);

/*calculates the keys for the current recursion using samples*/
void select_keys(int *keys, int fd,off_t size, int k);


/*return the bucket corresponding to comp*/
int bucket(int comp, int *key, off_t size);




