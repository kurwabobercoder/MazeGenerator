#!/bin/bash

echo "--- Compiling main.cpp and tester.cpp ---"
g++ main.cpp -o main
if [ $? -ne 0 ]; then
    echo "Compilation of main.cpp failed."
    exit 1
fi

# Ensure C++17 for structured bindings in tester.cpp
g++ tester.cpp -o tester -std=c++17
if [ $? -ne 0 ]; then
    echo "Compilation of tester.cpp failed."
    exit 1
fi

echo "--- Running 2000 tests ---"
passed_count=0
failed_count=0
timeout_count=0
runtime_error_count=0


for i in {1..1000}; do
    size=$(( RANDOM % 466 + 5 ))

    temp_maze_output=$(mktemp)

    if ! echo $size | gtimeout 5s ./main > "$temp_maze_output"; then
        echo "--- Test $i timed out or crashed with size $size ---"
        timeout_count=$((timeout_count + 1))
        rm "$temp_maze_output"
        continue
    fi

    result=$(cat "$temp_maze_output" | gtimeout 5s ./tester)
    exit_status=$?


    if [[ "$result" == *"Test passed!"* ]]; then
        passed_count=$((passed_count + 1))
    elif [ $exit_status -ne 0 ]; then
        echo "--- Test $i failed with a runtime error (exit status: $exit_status) with size $size ---"
        runtime_error_count=$((runtime_error_count + 1))
    else
        echo "--- Test $i failed with size $size ---"
        echo "$result"
        failed_count=$((failed_count + 1))
    fi

    rm "$temp_maze_output"
done

echo "--- Test Results Summary ---"
echo "Total tests: 2000"
echo "Passed: $passed_count"
echo "Failed: $failed_count"
echo "Timeouts: $timeout_count"
echo "Runtime Errors: $runtime_error_count"
