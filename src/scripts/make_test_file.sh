#!/bin/bash

if [ $1 ]; then
    SIZE=$1
else
    exit 1
fi

./make_rand $((SIZE/512))
