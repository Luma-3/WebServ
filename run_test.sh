#!/bin/bash

run_test() {
	./$1
	python3 $2
	kill -9 $(cat .server_pid)
	rm -f .server_pid
}

run_test "tests/python/run_classic_server.sh" "tests/python/request_return_code.py"
rm -f tests/python/components/no_perm.html