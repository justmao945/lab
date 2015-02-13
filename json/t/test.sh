#!/bin/bash

# compile tester

clang++ -g test.cc -I.. -o test

# run tests
for f in *.json
do
    echo $f
    ./test $f ${f%.json}.cmp
done

rm -f ./test
