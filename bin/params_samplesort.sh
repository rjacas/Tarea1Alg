#!/bin/bash
#rm -f ./results_sample*

M=26214400
N=$((256 * M))
max_size=$((2**10*M))

for j in 1
do
    echo "Making file with $N integers"
    if [ -e rand_src -a ! -e test_file.bak ]; then
        offset=$((((RANDOM % 100) * (max_size*4 - N*4))/51200))
        blocks=$(((N*4)/512))
    else 
        if [ ! -e test_file.bak ]; then
            ./make_test_file.sh $((4 * N))
            mv test_file test_file.bak
        fi
    fi

    for i in 1
    do
	if [ ! -e "test_file" ]; then
		if [ -e "rand_src" ]; then
	    	echo "Creating file with $blocks blocks; offset is $offset"
            	dd if=./rand_src of=test_file bs=512 skip=$offset count=$blocks
        	else
           	cp test_file.bak test_file
        	fi
	fi
        echo "Testing with $N integers: $j; k = $i"
        echo "=====================" >> "results_sample_$i"
        echo "Testing with $N integers: $j" >> "results_sample_$i"
        echo "=====================" >> "results_sample_$i"

        { time ./test_samplesort "test_file" $N "z" $i >> "results_sample_$i"; } 2>>"results_sample_$i"
	rm -f test_file
        rm  -f ./z*

    done
    rm -f test_file.bak
done
