#!/bin/bash

for test_dir in lab1_primjeri/test*; do
        test_name=$(basename "$test_dir")
        input_file="$test_dir/test.a"
        answer_file="$test_dir/test.b"

        output=$(./main < "$input_file")
        if [[ $output == $(<"$answer_file") ]];
        then
                echo "Test $test_name passed."
        else
                echo "Test $test_name failed."
        fi
done

