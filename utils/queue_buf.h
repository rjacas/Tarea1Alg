#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

struct queue_buf {
    int *elems;
    int size;
    int n_elems;
    int ptr;
};

/*
   Creates a new queue_buf structure, with capacity for
   size integers. 

Params:
    size(IN) - number of integers to reserve space for.
Returns:
    a newly allocated queue_buf structure. It must be
    freed with free_queue_buf.
*/
struct queue_buf *queue_buf_new(int size);

/*
   Refills a queue_buf structure with integers extracted
   from fd.

Params:
    q(IN) - the queue_buf structure to be refilled.
    fd(IN) - the file descriptor for the file to be used.
Returns:
    TRUE if the structure was successfully filled; FALSE
    if it was not filled (the queue_buf might be empty
    or partially filled).
*/
int refill_queue_buf(struct queue_buf *q, int fd);

/* 
   Dequeues the first element from the queue_buf. Unspecified
   behavior is to happen if there aren't any elements in the
   queue_buf.

Params:
    q(IN) - the queue_buf to dequeue from.

Returns:
    the first integer in the queue_buf, or unspecified
    behavior if there are no elements are in the queue_buf.
*/
int dequeue(struct queue_buf *q);

/*
   Checks if a queue_buf is empty.

Params:
    q(IN) - the queue_buf to check for emptiness.

Returns:
    true if the queue_buf is empty; false if it isn't.
*/
 
int empty(struct queue_buf *q);
