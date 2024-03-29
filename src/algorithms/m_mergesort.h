#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/sort_utils.h"
#ifdef ASDF
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
#endif
/* 
   Uses k-ary mergesort to sort a file, using at most l blocks
   per queue.

Params:
    fd1(IN/OUT) - the file descriptor for the file to sort. It must be at the start 
    of the file.
    fd2(IN/OUT) - the file descriptor for the auxiliary file to use. 
    k(IN) - number of queue_buf structures to use.
    N(IN) - the number of elements in the file to sort.

*/
struct sort_results  m_mergesort(int fd1, int fd2, int k, off_t N);

/* 
   Recursive function for m_mergesort. It divides the current chunk of the
   file to be sorted in k chunks. If the smaller chunks are of a size smaller
   than l, recursion is stopped and the chunks are sorted in memory.
   
Params:
    fd1(IN/OUT) - the file descriptor of the file to sort.
    fd2(IN/OUT) - the file descriptor of the auxiliary file to use.
    level(IN) - current recursion level
    start(IN) -  starting position of the chunk of the file to sort, in integers.
    size(IN) - size of the chunk of the file to sort, in integers.
*/
void mergesort(int fd, int fd2, int level, off_t start, off_t size);


