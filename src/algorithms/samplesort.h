#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <limits.h>


/* Amount of elements that will fit in memory */
#define M 26214400

/* Amount of elements to be sorted */
#define N 6710886400

/* Amount of elements that fit in a disk sector */
#define B 128

/* Number of sectors that fit in memory */
#define m M/B

/* Number of sectors used by the elements to be
   sorted */
/* FIXME shouldn't this have a ceil? */
#define n N/B

/* Length of the queue_bufs to be used, in sectors */
#define l m/k

/* Number of queue_bufs to be used */
#define k 4
/*log2(k)*/
#define a log(k)/log(2) 

struct bucket{

	int file;
	int size;
	char name[30];
	int nivel;
	int min;
	int max;

};

