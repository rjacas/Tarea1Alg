#!/bin/bash

optimum_k=8
rm -f results
touch results

M=512 #$((26214400))
set -e
for i in {1..10}
do
    N=$((2 ** i * M))
    echo "=====================" >> results
    echo "Testing with $N integers" >> results
    echo "=====================" >> results
    touch aux
    ./make_test_file.sh  $((4 * N))

    { time ./test_mergesort "test_file" $N aux 3 >> results; } 2>>results

    rm test_file
    rm aux

done

