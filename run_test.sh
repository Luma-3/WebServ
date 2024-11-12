#!/bin/bash

run_test() {
	./$1
	python3 $2 &

	wait $!

	kill -9 $(cat .server_pid)
	rm -f .server_pid
}

run_test "tests/python/run_classic_server.sh" "tests/python/request_return_code.py"
rm -f tests/python/components/no_perm.html

echo "Prepare environment for gtest"
echo

cp tests/gtest/all_run.sh build/tests/all_run.sh

cp -r tests/gtest/conf/ build/tests/
chmod 000  build/tests/conf/exception/no_perm.conf

echo "Prepare environment DONE"

echo "Running gtest"
echo

./build/tests/all_run.sh

echo "Cleaning up"
echo

chmod 644 build/tests/conf/exception/no_perm.conf
rm -rf build/tests/conf/

echo "Done"