#!/bin/bash

# compile tester

clang++ test.cc -I.. -ggdb3 -fno-inline -O0 -o test.exe

# run tests
for f in *.json
do
    echo $f
    ./test.exe $f ${f%.json}.cmp
done
