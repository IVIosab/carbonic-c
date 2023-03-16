#!/bin/bash

for folder in ./tests/components/*;
do 
    dir_name=${folder##*/}
    echo "Processing $dir_name tests..."
    for file in ./tests/components/$dir_name/*.crbc;
    do
        file_name=${file##*/} 
        file_base=${file_name%.crbc}
        echo -e -n "\tProcessing {$file_base} test... "
        ./output.out<$f>./tests/parser_outputs/${name}/${base}.txt
        echo "Completed!"
    done
    echo "Completed!"
done