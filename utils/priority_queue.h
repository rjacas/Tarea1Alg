struct pair {
    int i1, i2;
};

struct priority_queue {
    struct pair *elems;
};


/* 
   Creates a new priority_queue structure, which should be
   deallocated through pq_free().

Params:
    size(IN) - the number of elements the priority_queue will
    be able to hold.

Returns:
    the newly created priority_queue structure.
*/
struct priority_queue *pq_new(int size);

/*
    Inserts an element in the priority queue, along with an identifier
    for the origin of it. Unspecified behavior will happen if an
    insertion is made on a full priority queue.

Params:
    p(IN) - the priority queue where the element will be inserted.
    new_elem(IN) - the element to insert.
    origin(IN) - an identifier for the origin of the element.

*/
void pq_insert(struct priority_queue *p, int new_elem, int origin);

/*
   Extracts the smallest element and its origin identifier from a
   priority queue. Undefined behavior will happen if the smallest
   element from an empty priority queue is extracted.

Params: 
    p(IN) - the priority queue from which to extract the smallest
    element and its origin.
    elem(OUT) - pointer to where the smallest element in the
    priority queue should be left.
    origin(OUT) - pointer to where the origin identifier of the
    smallest element in the priority queue should be left.
*/

void pq_extract(struct priority_queue *p, int *elem, int *origin);

/*
   Deallocates a priority queue created through pq_new(). Unspecified
   behavior will happen if a null priority queue, an already freed
   priority_queue or a priority queue that wasn't allocated through
   pq_new() is freed with this function.

Params:
    p(IN/OUT) - the priority queue to be freed.
*/
void pq_free(struct priority_queue *p);

