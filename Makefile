


tests: tests/test_priority_queue tests/test_queue_buf tests/test_urandom

tests/test_mergesort: src/tests/test_mergesort.c src/utils/priority_queue.c src/utils/priority_queue.h src/utils/queue_buf.c src/utils/queue_buf.h src/algorithms/m_mergesort.c src/algorithms/m_mergesort.h 
	gcc -g src/utils/priority_queue.c src/utils/queue_buf.c src/algorithms/m_mergesort.c src/tests/test_mergesort.c -o tests/test_mergesort

tests/test_priority_queue: src/utils/priority_queue.c src/utils/priority_queue.h src/tests/test_pq.c
	gcc -g src/utils/priority_queue.c src/tests/test_pq.c -o tests/test_priority_queue

tests/test_queue_buf: src/utils/queue_buf.c src/utils/queue_buf.h src/tests/test_qb.c
	gcc -g src/utils/queue_buf.c src/tests/test_qb.c -o tests/test_queue_buffer

tests/test_urandom: src/tests/test_urandom.c
	gcc -g src/tests/test_urandom.c -o tests/test_urandom

clean:
	rm -f tests/test*
