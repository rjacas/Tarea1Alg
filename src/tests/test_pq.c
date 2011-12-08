#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include "../utils/priority_queue.h"

void main(int argc, char **argv) {
    struct priority_queue *p;
    int i, tmp1, tmp2;
    
    p = pq_new(20);

    for (i = 0; i < 20; i++) {
        tmp1 = rand() % 100;
        tmp2 = rand() % 100;

        printf("Inserting %d from %d\n",tmp1, tmp2);
        pq_insert(p, tmp1, tmp2);
    }

    for (i = 0; i < 20; i++) {
        pq_extract(p, &tmp1, &tmp2);
        printf("Valor: %d from %d\n", tmp1, tmp2);
    }
    pq_free(p);

    p = pq_new(3);
    pq_insert(p, -13, 0);
    pq_insert(p, -12, 0);
    pq_insert(p, -9, 0);
    pq_extract(p, &tmp1, &tmp2);
    printf("Valor: %d from %d\n", tmp1, tmp2);
    pq_extract(p, &tmp1, &tmp2);
    printf("Valor: %d from %d\n", tmp1, tmp2);
    pq_extract(p, &tmp1, &tmp2);
        printf("Valor: %d from %d\n", tmp1, tmp2);

}


