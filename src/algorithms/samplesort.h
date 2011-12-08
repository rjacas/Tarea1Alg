#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "m_mergesort.h"


#define M 26214400
#define N 6710886400
#define B 128
#define m M/B
#define n N/B
#define l m/k
#define k 4
#define a 2 //log2(k)
