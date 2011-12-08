#!/bin/sh

SIZE="80"

if [ $1 ]; then
    SIZE=$1
fi

head -c$SIZE /dev/urandom > test_file
