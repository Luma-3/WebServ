#!/bin/bash

GTEST_DIR="gtest"
REPO_DIR="gtest/googletest"

# Check if the directory exists
if [ -d "$REPO_DIR" ]; then
	echo "Google Test already installed in $GTEST_DIR, Updating..."
	cd $GTEST_DIR
	git pull
else
	echo "Google Test not found, Cloning..."
	cd $GTEST_DIR
	git clone https://github.com/google/googletest.git $REPO_DIR
fi

# Build the library
mkdir -p build
cd build

echo "Cmake Gemerating..."
cmake ..

echo "Building..."
make -j$(nproc)

echo "Google Test Compiled Successfully!"