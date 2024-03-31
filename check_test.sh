#!/bin/bash

MAIN_EXECUTABLE="./main.exe"
TEST_FOLDER="./lab1_primjeri"

for ((i=1; i<=30; i++)); do
    TEST_DIR="$TEST_FOLDER/test$i"
    INPUT_FILE="$TEST_DIR/test.a"
    EXPECTED_OUTPUT_FILE="$TEST_DIR/test.b"

    if [ ! -f "$INPUT_FILE" ]; then
        continue
    fi

    OUTPUT=$("$MAIN_EXECUTABLE" < "$INPUT_FILE")

    if [ ! -f "$EXPECTED_OUTPUT_FILE" ]; then
        continue
    fi

    EXPECTED_OUTPUT=$(< "$EXPECTED_OUTPUT_FILE")
    if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
        echo "Test $i passed."
    else
        echo "Test $i failed."
    fi
done
