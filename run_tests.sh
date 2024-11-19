#!/bin/bash

google_test() {
	echo "Prepare environment for gtest"
	echo

	cp tests/gtest/all_run.sh build/tests/all_run.sh

	cp -rf tests/gtest/conf/ build/tests/
	chmod 000  build/tests/conf/lexer/exception/no_perm.conf

	echo "Prepare environment DONE"

	echo "Running gtest"
	echo

	./build/tests/all_run.sh || exit 1

	echo "Cleaning up"
	echo

	chmod 644 build/tests/conf/lexer/exception/no_perm.conf
	rm -rf build/tests/conf/

	echo "Done"
}

run_python_test() {
	echo "Running python tests"
	./tests/python/run_all_tests_python.sh || exit 1

	if [ "$TO_CLEAN" == "-y" ]; then
		rm -f log/*units_python_test*.log
	fi
}

TO_TEST=$1
TO_CLEAN=$2

if [ "$TO_TEST" == "all" ]; then
	echo "Running all tests"
	google_test
	run_python_test
elif [ "$TO_TEST" == "gtest" ]; then
	echo "Running gtest"
	google_test
elif [ "$TO_TEST" == "python" ]; then
	echo "Running python tests"
	run_python_test
elif [ "$TO_TEST" == "help" ]; then
	echo "Usage: $0 [all|gtest|python] {-y}"
	exit 1
else
	echo "Usage: $0 [all|gtest|python] {-y}"
	exit 1
fi

exit 0
