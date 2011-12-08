algorithms: algorithms/m_mergesort

algorithms/m_mergesort: src/utils/priority_queue.c src/utils/priority_queue.h src/utils/queue_buf.c src/utils/queue_buf.h src/algorithms/m_mergesort.c src/algorithms/m_mergesort.h
	gcc -g src/utils/priority_queue.c src/utils/queue_buf.c src/algorithms/m_mergesort.c -o algorithms/m_mergesort

tests: tests/test_priority_queue tests/test_queue_buf tests/test_mixed

tests/test_priority_queue: src/utils/priority_queue.c src/utils/priority_queue.h src/tests/test_pq.c
	gcc -g src/utils/priority_queue.c src/tests/test_pq.c -o tests/test_priority_queue

tests/test_queue_buf: src/utils/queue_buf.c src/utils/queue_buf.h src/tests/test_qb.c
	gcc -g src/utils/queue_buf.c src/tests/test_qb.c -o tests/test_queue_buffer

tests/test_mixed: src/tests/test_mixed.c src/utils/test_utils.h src/utils/test_utils.c
	gcc -g src/utils/test_utils.c src/tests/test_mixed.c -o tests/test_mixed

clean:
	rm -f tests/test*
