#!/bin/bash

for test_dir in lab2_primjeri/test*; do
        test_name=$(basename "$test_dir")
        input_file="$test_dir/t.ul"
        answer_file="$test_dir/t.iz"

        output=$(./main.exe < "$input_file")
        normalized_output=$(echo "$output" | tr -d '[:space:]')
        normalized_answer=$(cat "$answer_file" | tr -d '[:space:]')
        if [[ "$normalized_output" == "$normalized_answer" ]]; 
        then
                echo "Test $test_name passed."
        else
                echo "Test $test_name failed."
                echo -e "Output: \n$output"
                echo "Answer:"
                cat "$answer_file"
        fi
done

