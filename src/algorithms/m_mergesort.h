#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define M 26214400
#define N 6710886400
#define B 128
#define m M/B
#define n N/B
#define l m/k
#define k 4
#define N_ELEMS 10

/* 
   Uses in-place k-ary mergesort to sort a file, using at most l blocks
   per queue.
*/

void m_mergesort(int fd);

void mergesort(int fd, int size, int pos);
void m_quicksort(int fd, int size, int pos);
void quickaux(int begin, int end,int *buf);


void swap(int *a, int *b);
void sort(int *arr, int beg, int end);
