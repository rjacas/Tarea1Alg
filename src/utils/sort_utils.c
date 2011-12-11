#include "sort_utils.h"


void alpha_quicksort(int *buf, off_t size){
  sort(buf, 0, size);
}

void quicksort(struct queue_buf *q){
    int *buff;
    int size;
    qb_get_array(q, &buff, &size);
    sort(buff, 0, size);
}

void swap(int *a, int *b){
    int t=*a; *a=*b; *b=t;
}

void sort(int *arr, int beg, int end){
    int piv;
    int q;
    int r;
    if(end > beg + 1){
        piv = arr[beg], q = beg + 1, r = end;
        while(q < r){
            if (arr[q] <= piv)
                q++;
            else
                swap(&arr[q], &arr[--r]);
        }
        swap(&arr[--q], &arr[beg]);
        sort(arr, beg, q);
        sort(arr, r, end);
    }
}

