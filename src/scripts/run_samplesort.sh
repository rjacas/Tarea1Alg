#!/bin/bash

k=2
M=104857600
max_n=10
n_it=$((max_n-1))
max_size=$((2**max_n * M))

#if [ ! -e "./rand_src" ]; then
#    echo "Creating file with $max_size bytes"
#    ./make_test_file $max_size
#    mv ./test_file ./rand_src
#fi



for i in `seq 1 $n_it`;
do

    test_size=$((2**i * M))
    
    #echo "Creating file of size $test_size bytes; mod = $((test_size % 512))"
    #offset=$((((RANDOM % 100) * (max_size - test_size))/51200))

    #blocks=$((test_size/512))
    #dd if=./rand_src of=./test_file bs=512 skip=$offset count=$blocks

    echo "=====================" >> "results"
    echo "Testing with i=$i" >> "results"
    echo "=====================" >> "results"
    echo "Testing with 2**$i * $M = $test_size bytes"
    { time ./test_samplesort "test_file" $((test_size/4)) "t" $k >>results; } 2>> results

    echo >> results
done
