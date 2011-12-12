#!/bin/bash

k=11
M=104857600
max_n=10
n_it=$((max_n-1))
max_size=$((2**max_n * M))

if [ ! -e "./rand_src" ]; then
    echo "Creating file with $max_size bytes"
    ./make_test_file $max_size
    mv ./test_file ./rand_src
fi

rm -f test_file
rm -f aux

for i in `seq 1 $n_it`;
do

    test_size=$((2**i * M))
    
    echo "Creating file of size $test_size bytes; mod = $((test_size % 512))"
    offset=$((((RANDOM % 100) * (max_size - test_size))/51200))

    blocks=$((test_size/512))
    dd if=./rand_src of=./test_file bs=512 skip=$offset count=$blocks

    touch aux

    echo "Testing with 2**$i * $M = $test_size bytes"
    ./test_mergesort "test_file" $test_size aux $k
    rm -f test_file
    rm -f aux
done