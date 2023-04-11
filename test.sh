#!/bin/bash

test_files () {
    for file in $1*.crbc;
    do
        file_name=${file##*/} 
        file_base=${file_name%.crbc}
        echo -e "\t\tProcessing {$file_base} test... "
        
        # Create output file if not exists
        [ ! -f $2${file_base}.txt ] && touch $2${file_base}.txt

        ./output.out < $file > $2${file_base}.txt
        echo -e "\t\tCompleted!"
    done
}

# Check if test dirs exist
[ ! -d tests ] && echo "Error: tests folder not found!\n" && exit 1
[ ! -d tests/inputs ] && echo "Error: tests/inputs folder not found!\n" && exit 1
[ ! -d tests/inputs/components ] && echo "Error: tests/inputs/components folder not found!\n" && exit 1
[ ! -d tests/inputs/invalid ] && echo "Error: tests/inputs/invalid folder not found!\n" && exit 1

# Create output dir
[ ! -d tests/outputs ] && mkdir tests/outputs

for type in ./tests/inputs/*;
do
    type_name=${type##*/}

    # Create output dir if not exists
    [ ! -d tests/outputs/$type_name ] && mkdir tests/outputs/$type_name

    echo "Processing {$type_name} tests..."
    for dir in ./tests/inputs/$type_name/*;
    do 
        dir_name=${dir##*/}

        # Create output dir if not exists
        [ ! -d tests/outputs/$type_name/$dir_name ] && mkdir tests/outputs/$type_name/$dir_name

        echo -e "\tProcessing $dir_name tests..."

        if [ $dir_name == "types" ]; 
        then 
            for inner_dir in ./tests/inputs/$type_name/types/*;
            do
                inner_dir_name=${inner_dir##*/}

                 # Create output dir if not exists
                [ ! -d tests/outputs/$type_name/$dir_name/$inner_dir_name ] && mkdir tests/outputs/$type_name/$dir_name/$inner_dir_name

                test_files ./tests/inputs/$type_name/types/$inner_dir_name/ ./tests/outputs/${type_name}/${dir_name}/${inner_dir_name}/

            done
        else

            test_files ./tests/inputs/$type_name/$dir_name/ ./tests/outputs/${type_name}/${dir_name}/
        
        fi
        echo -e "\tCompleted!"
    done
    echo "Completed!"
done
