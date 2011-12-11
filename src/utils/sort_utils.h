#include "queue_buf.h"

struct sort_results {
    long long io_acc;
    long long io_rand;
};

void quicksort(struct queue_buf *q);
void swap(int *a, int *b);
void sort(int *arr, int beg, int end);
