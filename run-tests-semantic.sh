#!/bin/bash

for i in {1..9}; do
    if [ $i -lt 10 ]; then
        filename="./assg2-milestone1/semantic-tests/tests/err0${i}"
    else
        filename="./assg2-milestone1/semantic-tests/tests/err${i}"
    fi

    echo -e "\nRunning test: $filename"
    ./compile --chk_decl < "$filename"
done

for i in {1..6}; do
    if [ $i -lt 10 ]; then
        filename="./assg2-milestone1/semantic-tests/tests/test0${i}"
    else
        filename="./assg2-milestone1/semantic-tests/tests/test${i}"
    fi

    echo -e "\nRunning test: $filename"
    ./compile --chk_decl < "$filename"
done
