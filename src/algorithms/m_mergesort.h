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


void m_mergesort(int fd);

void mergesort(int fd, int size, int pos);

void m_quicksort(int fd, int size, int pos);

void quickaux(int begin, int end,int *buf);
