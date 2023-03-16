#!/bin/bash


for type in ./tests/inputs/*;
do
    type_name=${type##*/}
    echo "Processing {$type_name} tests..."
    for dir in ./tests/inputs/$type_name/*;
    do 
        dir_name=${dir##*/}
        echo -e "\tProcessing $dir_name tests..."
        for file in ./tests/inputs/$type_name/$dir_name/*.crbc;
        do
            file_name=${file##*/} 
            file_base=${file_name%.crbc}
            echo -e -n "\t\tProcessing {$file_base} test... "
            ./output.out<$f>./tests/outputs/${type_name}/${dir_name}/${file_base}.txt
            echo "Completed!"
        done
        echo -e "\tCompleted!"
    done
    echo "Completed!"
done
