tests: tests/test_priority_queue tests/test_queue_buf

tests/test_priority_queue: src/utils/priority_queue.h src/utils/priority_queue.h src/tests/test_pq.c
	gcc -g src/utils/priority_queue.c src/tests/test_pq.c -o tests/test_priority_queue

tests/test_queue_buf: src/utils/queue_buf.h src/utils/queue_buf.h src/tests/test_qb.c
	gcc -g src/utils/queue_buf.c src/tests/test_qb.c -o tests/test_queue_buffer

clean:
	rm -f tests/*
