#ifndef _QUEQUE_BUF_
#define _QUEQUE_BUF_
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

struct queue_buf {
    /* The data contained in the queue_buf */
    int *elems;

    /* The amount of data, in integers, that the queue_buf
       can hold */
    int size;

    /* The current number of elements in the queue_buf */
    int n_elems;

    /*Position of the first non-null element in the queue_buf */
    int first;
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
struct queue_buf *qb_new(int size);

/*
   Refills a queue_buf structure with integers extracted
   from a file. It assumes the queue_buf is empty: if called
   on a non-empty queue_buf, the old data will be deleted.

Params:
    q(IN/OUT) - the queue_buf structure to be refilled.
    fd(IN/OUT) - the file descriptor for the file to be used.
Returns:
    TRUE if the structure was successfully filled; FALSE
    if it was not filled (the queue_buf might be empty
    or partially filled).
*/
int qb_refill(struct queue_buf *q, int fd);

/*
   Refills a queue_buf structure with integers extracted
   from a file. It assumes the queue_buf is empty: if called
   on a non-empty queue_buf, the old data will be deleted.
   At most max_bytes are read.

Params:
    q(IN/OUT) - the queue_buf structure to be refilled.
    fd(IN/OUT) - the file descriptor for the file to be used.
    max_bytes(IN) - maximum amount of bytes to be read from fd.

Returns:
    TRUE if the structure was successfully filled; FALSE
    if it was not filled (the queue_buf might be empty
    or partially filled).
*/
int qb_refill_max(struct queue_buf *q, int fd, int max_bytes);



/*
   Writes the content of a queue_buf to a file.
   Only the elements present in the queue buf will be written
   (therefore, for example, flushing an empty queue_buf will
   do nothing). After calling this function, the queue_buf should
   be treated as empty. Currently incompatible with dequeue().

Params:
    q(IN) - the queue_buf structure to be flushed.
    fd(IN) - the file descriptor for the file to be written to.
    Data will be written at the file descriptors' current position.

Returns:
    The number of bytes actually written to the file. In case of error, 
    -1 will be returned and errno will be changed to an appropiate
    value (same behavior as that of write(): see man 2 write)
*/
int qb_flush(struct queue_buf *q, int fd);

/*
   Enqueues an integer at the end of the queue_buf.
   Unspecified behavior will happen if this function
   is called on a full queue_buf. Currently incompatible
   with dequeue().

Params:
    q(IN) - the queue_buf where the element will be
    enqueued.

    new_elem(IN) - the element to be enqueued.

*/
void qb_enqueue(struct queue_buf *q, int new_elem);

/* 
   Dequeues the first element from the queue_buf. Unspecified
   behavior is to happen if there aren't any elements in the 
   queue_buf. Currently incompatible with enqueue() and flush().

Params:
    q(IN) - the queue_buf to dequeue from.

Returns:
    the first integer in the queue_buf, or unspecified
    behavior if there are no elements are in the queue_buf.
*/
int qb_dequeue(struct queue_buf *q);

/*
   Returns the array of elements from a queue_buf and its size.
   It will not be a shallow copy, so the queue_buf should not be
   deallocated while the returned array is used.

Params:
    q(IN) - the queue_buf to get the elements from.
    arr(OUT) - pointer to where to leave the array of elements.
    size(OUT) - pointer to where to leave the number of elements
    extracted.

*/
void qb_get_array(struct queue_buf *q, int **arr, int *size);

/*
   Checks if a queue_buf is empty.

Params:
    q(IN) - the queue_buf to check for emptiness.
 
Returns:
    TRUE if the queue_buf is empty; FALSE if it isn't.
*/
 
int qb_empty(struct queue_buf *q);

/*
   Checks if a queue_buf is full.

Params:
    q(IN) - the queue_buf to check for fullness.

Returns:
    TRUE if the queue_buf is full; FALSE if it isn't.
*/
 
int qb_full(struct queue_buf *q);

#endif
