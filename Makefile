default: bin tests

bin: bin/run_mergesort bin/run_samplesort bin/make_rand
	cp src/scripts/*.sh bin/

tests: tests/test_mergesort tests/test_samplesort tests/test_priority_queue tests/test_queue_buf tests/test_mixed
	cp src/scripts/*.sh tests/

bin/run_mergesort: src/scripts/run_mergesort.c src/utils/sort_utils.c src/utils/sort_utils.h src/utils/priority_queue.c src/utils/priority_queue.h src/utils/queue_buf.c src/utils/queue_buf.h src/algorithms/m_mergesort.c src/algorithms/m_mergesort.h
	gcc -D_FILE_OFFSET_BITS=64 src/utils/sort_utils.c src/utils/priority_queue.c src/utils/queue_buf.c src/algorithms/m_mergesort.c src/scripts/run_mergesort.c -o bin/test_mergesort 

bin/run_samplesort: src/scripts/run_samplesort.c src/utils/sort_utils.c src/utils/sort_utils.h src/utils/test_utils.c src/utils/queue_buf.c src/utils/queue_buf.h src/algorithms/alpha_samplesort.c src/algorithms/alpha_samplesort.h
	gcc -g -lm -D_FILE_OFFSET_BITS=64 -DDEBUG src/utils/test_utils.c src/utils/sort_utils.c src/utils/priority_queue.c src/utils/queue_buf.c src/algorithms/alpha_samplesort.c src/scripts/run_samplesort.c -o bin/test_samplesort

bin/make_rand: src/scripts/make_rand.c
	gcc -D_FILE_OFFSET_BITS=64 src/scripts/make_rand.c -o bin/make_rand

tests: tests/test_mergesort tests/test_priority_queue tests/test_queue_buf tests/test_mixed tests/test_flush

tests/test_mergesort: src/utils/sort_utils.c src/utils/sort_utils.h src/utils/test_utils.c src/utils/priority_queue.c src/utils/priority_queue.h src/utils/queue_buf.c src/utils/queue_buf.h src/algorithms/m_mergesort.c src/algorithms/m_mergesort.h src/tests/test_mergesort.c
	gcc -g -D_FILE_OFFSET_BITS=64 -DDEBUG src/tests/test_mergesort.c src/utils/sort_utils.c src/utils/test_utils.c src/utils/priority_queue.c src/utils/queue_buf.c src/algorithms/m_mergesort.c -o tests/test_mergesort

tests/test_samplesort: src/utils/sort_utils.c src/utils/sort_utils.h src/utils/test_utils.c src/utils/priority_queue.c src/utils/priority_queue.h src/utils/queue_buf.c src/utils/queue_buf.h src/algorithms/alpha_samplesort.c src/algorithms/alpha_samplesort.h src/tests/test_samplesort.c
	gcc -g -lm -D_FILE_OFFSET_BITS=64 -DDEBUG src/utils/test_utils.c src/utils/sort_utils.c src/tests/test_samplesort.c src/utils/priority_queue.c src/utils/queue_buf.c src/algorithms/alpha_samplesort.c -o tests/test_samplesort

tests/test_priority_queue: src/utils/priority_queue.c src/utils/priority_queue.h src/tests/test_pq.c
	gcc -g -DDEBUG src/utils/priority_queue.c src/tests/test_pq.c -o tests/test_priority_queue

tests/test_queue_buf: src/utils/queue_buf.c src/utils/queue_buf.h src/tests/test_qb.c
	gcc -g  -DDEBUG src/utils/queue_buf.c src/tests/test_qb.c -o tests/test_queue_buffer

tests/test_flush: src/utils/queue_buf.c src/utils/queue_buf.h src/tests/test_flush.c
	gcc -g -DDEBUG src/utils/test_utils.c src/utils/queue_buf.c src/tests/test_flush.c -o tests/test_flush

tests/test_mixed: src/tests/test_mixed.c src/utils/test_utils.h src/utils/test_utils.c
	gcc -g -D_FILE_OFFSET_BITS=64 -DDEBUG src/utils/test_utils.c src/tests/test_mixed.c -o tests/test_mixed


clean:
	rm -f tests/test*
	rm -f tests/*.sh
	rm -f tests/aux
	rm -f bin/*.sh
	rm -f bin/test*
	rm -f bin/*.sh
	rm -f bin/aux
	rm -f bin/results
