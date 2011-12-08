#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/queue_buf.h"
#include "../utils/priority_queue.h"

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

/* 
   Uses in-place k-ary mergesort to sort a file, using at most l blocks
   per queue.

Params:
    fd(IN/OUT) - the file descriptor of the file to sort. It must be at the start 
    of the file.

*/
void m_mergesort(int fd);

/* 
   Recursive function for m_mergesort. It divides the current chunk of the
   file to be sorted in k chunks. If the smaller chunks are of a size smaller
   than l, recursion is stopped and the chunks are sorted in memory.
   
Params:
    fd(IN/OUT) - the file descriptor of the file to sort. Its position is implicit.
    size -  size of the chunk of the file to sort, in sectors.
    file_pos - position in the file to start reading from, in bytes.
*/
void mergesort(int fd, int size, int child_idx, int file_pos);

void m_quicksort(struct queue_buf *q);


void swap(int *a, int *b);
void sort(int *arr, int beg, int end);
