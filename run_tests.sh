#!/bin/bash

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

./tests/python/run_all_tests_python.sh

echo "Do you want to delete log file generated during the test? [y/n]"
read answer
if [ "$answer" == "y" ]; then
	rm -f log/*.log
fi