#!/bin/bash

rm -f ./results*

M=26214400
N=$((256 * M))
set -e

for j in {1..5}
do
    echo "Making file with $N integers"
    if [ ! -e test_file.bak ]; then
        ./make_test_file.sh $((4 * N))
        cp test_file test_file.bak
    fi

    for i in {2..4}
    do
        echo "Testing with $N integers: $j; k = $i"
        echo "=====================" >> "results_$i"
        echo "Testing with $N integers: $j" >> "results_$i"
        echo "=====================" >> "results_$i"
        touch aux

        { time ./test_mergesort "test_file" $N aux $i >> "results_$i"; } 2>>"results_$i"

        rm test_file
        cp test_file.bak test_file
        rm aux

    done
    rm test_file.bak
done
