#!/bin/bash

run_test() {

	echo "Running test $1"
	echo
	./$1 &
	PID=$!

	wait $PID
	EXIT_CODE=$?

	if [ $EXIT_CODE -eq 0 ]
	then
		echo "Test passed."
		TESTS_PASSED=$((TESTS_PASSED+1))
	else
		echo "Test failed."
		TESTS_FAILED=$((TESTS_FAILED+1))
	fi
}

TESTS_PASSED=0
TESTS_FAILED=0

SRCPIT_DIR=$(dirname $(realpath $0))

cd "$SRCPIT_DIR" || { echo "Failed to change directory!" && exit 1; }

ALL_TESTS=$(find . -name "*.out" -type f)


for test in $ALL_TESTS
do
	run_test $test
done

echo "Main Tests passed: $TESTS_PASSED"
echo "Main Tests failed: $TESTS_FAILED"
exit $TESTS_FAILED