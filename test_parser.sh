#!/bin/bash

for f in ./tests/valid_tests/*.crbc;
do 
    name=${f##*/}
    base=${name%.crbc}
    echo "Processing $base test..."
    ./a.out<$f>./tests/parser_outputs/${base}.txt
    
done