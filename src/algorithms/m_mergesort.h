#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/queue_buf.h"
#include "../utils/priority_queue.h"

#define M 26214400
#define N 6710886400
#define B 128
#define m M/B
#define n N/B
#define l m/k
#define k 4
#define N_ELEMS 100

/* 
   Uses in-place k-ary mergesort to sort a file, using at most l blocks
   per queue.

Params:
    fd(IN/OUT) - the file descriptor of the file to sort. It must be at the start 
    of the file.

*/
void m_mergesort(int fd);

/* 
   Recursive function for m_mergesort. It divides the current sector of the
   file to be sorted in k sectors. If the smaller sectors are of a size smaller
   than l, the recursion is stopped and the sectors are sorted in memory.
   
Params:
    fd(IN/OUT) - the file descriptor of the file to sort. Its position is implicit.
    size -  size of the sector of the file to sort.
    pos - ???
*/
void mergesort(int fd, int size, int pos);

void m_quicksort(struct queue_buf *q);
void quickaux(int begin, int end,int *buf);


void swap(int *a, int *b);
void sort(int *arr, int beg, int end);
