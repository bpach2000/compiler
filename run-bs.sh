#!/bin/bash

for i in {1..9}; do
    filename="./Grading-Script-453-main/TestParsing/test0${i}.in"

    echo -e "\nRunning test: $filename"
    ./compile --chk_decl < "$filename"
done
