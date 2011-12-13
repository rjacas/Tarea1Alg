default: bin tests clean_o

bin: bin/run_mergesort bin/run_samplesort bin/make_rand
	cp src/scripts/*.sh bin/

bin/run_mergesort: src/scripts/run_mergesort.c su.o pq.o qb.o merge.o 
	gcc -D_FILE_OFFSET_BITS=64 su.o pq.o qb.o merge.o src/scripts/run_mergesort.c -o bin/test_mergesort 

bin/run_samplesort: src/scripts/run_samplesort.c su.o tu.o qb.o sample.o
	gcc -g -lm -D_FILE_OFFSET_BITS=64 -DDEBUG su.o tu.o qb.o sample.o src/scripts/run_samplesort.c -o bin/test_samplesort

bin/make_rand: src/scripts/make_rand.c
	gcc -D_FILE_OFFSET_BITS=64 src/scripts/make_rand.c -o bin/make_rand

tests: tests/test_mergesort tests/test_samplesort tests/test_priority_queue tests/test_queue_buf tests/test_mixed
	cp src/scripts/*.sh tests/

tests/test_mergesort: src/tests/test_mergesort.c merge.o qb.o pq.o su.o tu.o
	gcc -g -lm -D_FILE_OFFSET_BITS=64 -DDEBUG merge.o qb.o pq.o su.o tu.o src/tests/test_mergesort.c  -o tests/test_mergesort

tests/test_samplesort: src/tests/test_samplesort.c sample.o qb.o pq.o su.o tu.o
	gcc -g -lm -D_FILE_OFFSET_BITS=64 -DDEBUG sample.o qb.o pq.o su.o tu.o src/tests/test_samplesort.c -o tests/test_samplesort

tests/test_priority_queue: src/tests/test_pq.c pq.o
	gcc -g -DDEBUG pq.o src/tests/test_pq.c -o tests/test_priority_queue

tests/test_queue_buf: src/tests/test_qb.c qb.o
	gcc -g  -DDEBUG qb.o src/tests/test_qb.c -o tests/test_queue_buffer

tests/test_flush: src/tests/test_flush.c qb.o tu.o
	gcc -g -DDEBUG tu.o qb.o src/tests/test_flush.c -o tests/test_flush

tests/test_mixed: src/tests/test_mixed.c tu.o
	gcc -g -D_FILE_OFFSET_BITS=64 -DDEBUG tu.o src/tests/test_mixed.c -o tests/test_mixed

merge.o: src/algorithms/m_mergesort.c src/algorithms/m_mergesort.h tu.o qb.o pq.o
	gcc -c tu.o qb.o pq.o src/algorithms/m_mergesort.c -o merge.o

sample.o: src/algorithms/alpha_samplesort.c src/algorithms/alpha_samplesort.h tu.o qb.o pq.o
	gcc -c tu.o qb.o pq.o src/algorithms/alpha_samplesort.c -o sample.o
	
tu.o: src/utils/test_utils.c src/utils/test_utils.h
	gcc -c src/utils/test_utils.c -o tu.o

su.o: src/utils/sort_utils.c src/utils/sort_utils.h qb.o
	gcc -c src/utils/sort_utils.c qb.o -o su.o

qb.o: src/utils/queue_buf.c src/utils/queue_buf.h
	gcc -c src/utils/queue_buf.c -o qb.o

pq.o: src/utils/priority_queue.c src/utils/priority_queue.h
	gcc -c src/utils/priority_queue.c -o pq.o
  
clean:
	rm -f tests/test*
	rm -f tests/*.sh
	rm -f tests/aux
	rm -f bin/*.sh
	rm -f bin/test*
	rm -f bin/*.sh
	rm -f bin/aux
	rm -f bin/results

clean_o:
	rm -f ./*.o
