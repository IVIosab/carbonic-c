#!/bin/bash

for f in ./tests/valid_tests/*.crbc;
do 
    name=${f##*/}
    base=${name%.crbc}
    echo "Processing $base test..."
    ./lexer.out<$f>./tests/lexer_outputs/${base}.txt
    
done