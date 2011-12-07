#include <stdlib.h>
#include "priority_queue.h"

struct priority_queue *pq_new(int size) {
    struct priority_queue *p;

    p = (struct priority_queue *)malloc(sizeof(struct priority_queue));
    p->elems = (struct pair*) malloc (sizeof(struct pair)*(size + 1));
    p->elems[0].i1 = 0;
    p->elems[0].i2 = size;
    return p;
}

int pq_empty(struct priority_queue *p) {
    return ((p->elems[0].id1 == 0)? TRUE : FALSE);
}

void pq_insert(struct priority_queue *pq, int new_elem, int origin) {
    int i;
    struct pair tmp;

#ifdef DEBUG
    if ((pq->elems[0].i1 + 1) == pq->elems[0].i2) {
        printf("Max elems in priority queue exceeded.\n");
        exit(1);
    }
#endif

    pq->elems[0].i1++;
    pq->elems[pq->elems[0].i1].i1 = new_elem;
    pq->elems[pq->elems[0].i1].i2 = origin;

    for (i = pq->elems[0].i1; i > 1 && pq->elems[i].i1 <= pq->elems[i/2].i1; i /= 2) {
        tmp.i1 = pq->elems[i].i1;
        pq->elems[i].i1 = pq->elems[i/2].i1;
        pq->elems[i/2].i1 = tmp.i1;
        tmp.i2 = pq->elems[i].i2;
        pq->elems[i].i2 = pq->elems[i/2].i2;
        pq->elems[i/2].i2 = tmp.i2;
   }
}

void pq_extract(struct priority_queue *pq, int *elem, int *origin) {
    int tmp, i, j;

#ifdef DEBUG
    if (pq->elems[0].id1 == 0) {
        printf("No elements to extract from priority queue.\n");
        exit(1);
    }
#endif
    *elem = pq->elems[1].i1;
    *origin = pq->elems[1].i2;

    pq->elems[1].i1 = pq->elems[pq->elems[0].i1].i1;
    pq->elems[1].i2 = pq->elems[pq->elems[0].i1].i2;
    pq->elems[0].i1--;
    i = 1;

    while (2 * i < pq->elems[0].i1) {
        j = 2 * i;
        if (j + 1 < pq->elems[0].i1 && pq->elems[j+1].i1 <= pq->elems[j].i1) j++;

        if (pq->elems[i].i1 < pq->elems[j].i1) break;

        tmp = pq->elems[i].i1;
        pq->elems[i].i1 = pq->elems[j].i1;
        pq->elems[j].i1 = tmp;

        tmp = pq->elems[i].i2;
        pq->elems[i].i2 = pq->elems[j].i2;
        pq->elems[j].i2 = tmp;

        i = j;
    }
    return;
}

void pq_free(struct priority_queue *p) {
    free(p->elems);
    free(p);
}
