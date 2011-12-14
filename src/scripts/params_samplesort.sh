#!/bin/bash
set -x

M=26214400
N=$((256 * M))

for j in 1
do
    echo "Making file with $N integers"
  	if [ ! -e test_file.bak ]; then
        ./make_test_file.sh $((4 * N))
       cp test_file test_file.bak
    fi

    for i in 1 2 4
    do
        echo "Testing with $N integers: $j; k = $i"
        echo "=====================" >> "results_$i"
        echo "Testing with $N integers: $j" >> "results_$i"
        echo "=====================" >> "results_$i"

        { time ./test_samplesort "test_file" $N "z" $i >> "results_$i"; } >>"results_$i"
				
        cp test_file.bak test_file
        rm -f ./z*

    done
    rm test_file.bak
done
