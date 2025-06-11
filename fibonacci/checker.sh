#!/bin/bash

# Step 1: Compile the program (change main.cpp to your file if necessary)
g++ -o my_executable ../interpreter.cpp ../CPU.cpp ../Instruction.cpp ../Memory.cpp
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

EXECUTABLE=./my_executable
INPUT_DIR="input"
OUTPUT_DIR="output"
RECEIVED_FILE="output.txt"

all_passed=true

for testcase in "$INPUT_DIR"/input*.txt; do
    testcase_name="$testcase"
    num=$(echo "$testcase" | grep -oP '\d+\.txt')
    output_file="$OUTPUT_DIR/output${num}"
    # Prepare input.txt
    cat > input.txt <<EOF
1
2
fib.txt
2
2
$testcase_name
3
EOF

    # Run the program
    $EXECUTABLE < input.txt

    if [[ -f "$output_file" && -f "$RECEIVED_FILE" ]]; then
        # Compare the output files
        if diff -q <(sort "$output_file") <(sort "$RECEIVED_FILE") > /dev/null; then
            rm "$RECEIVED_FILE"
            # echo "Testcase $num: Passed"
        else
            echo "Testcase $num: Failed"
            echo "Expected output:"
            cat "$output_file"
            echo "Received output:"
            cat "$RECEIVED_FILE"
            all_passed=false
        fi
    else
        echo "Output file for testcase $num does not exist."
        all_passed=false
    fi
done

if $all_passed; then
    echo "All outputs matched expected results!"
else
    echo "Some testcases failed. See above for details."
fi