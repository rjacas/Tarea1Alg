#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>
#include "../utils/sort_utils.h"


/* Amount of elements that will fit in memory */
#define M 26214400

/* Amount of elements to be sorted */
//#define N 6710886400
#define N 805306368
/* Amount of elements that fit in a disk sector */
#define B 128


struct sort_results alpha_samplesort(int fd, off_t size,char *base_name,int k);

void s_samplesort(int fd,off_t size,char *base_name,int k);

/*calculates the keys for the current recursion using samples*/
void select_keys(int *keys, int fd,off_t size, int k);


/*return the bucket corresponding to comp*/
int bucket(int comp, int *key, off_t size);




