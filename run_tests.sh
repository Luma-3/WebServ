#!/bin/bash

./tests/python/run_all_tests_python.sh

echo "Do you want to delete log file generated during the test? [y/n]"
read answer
if [ "$answer" == "y" ]; then
	rm -f log/*.log
fi
# run_test "tests/python/run_classic_server.sh" "tests/python/request_return_code.py"