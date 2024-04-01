#!/bin/bash

for i in {1..40}; do
    if [ $i -lt 10 ]; then
        filename="./assg2-milestone1/syntax-tests/tests/err0${i}"
    else
        filename="./assg2-milestone1/syntax-tests/tests/err${i}"
    fi

    echo -e "\nRunning test: $filename"
    ./compile < "$filename"
done

for i in {1..15}; do
    if [ $i -lt 10 ]; then
        filename="./assg2-milestone1/syntax-tests/tests/test0${i}"
    else
        filename="./assg2-milestone1/syntax-tests/tests/test${i}"
    fi

    echo -e "\nRunning test: $filename"
    ./compile < "$filename"
done
