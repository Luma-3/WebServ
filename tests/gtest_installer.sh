#!/bin/bash

project_root=$(dirname $(dirname $(realpath $0)))

if [ "$PWD" != "$project_root" ]; then
	echo "Please run this script from the project root directory"
	exit 1
fi

TEST_DIR="tests"
GTEST_DIR="gtest"
REPO_DIR="googletest"

REPO_URL="https://github.com/google/googletest.git"

cd "$TEST_DIR"/"$GTEST_DIR" || { echo "Failed to change directory!" && exit 1; }

# Check if the directory exists
if [ -d "$REPO_DIR" ]; then
	echo "Google Test already installed in "$GTEST_DIR", Updating..."
	{ 
		cd "$REPO_DIR" && git pull; 
	} || {
		echo "Failed to update Google Test!" && exit 1;
	}
else
	echo "Google Test not found, Cloning..."
	git clone "$REPO_URL" "$REPO_DIR" || { echo "Failed to clone Google Test!" && exit 1; }
	cd "$REPO_DIR" || { echo "Failed to change directory!" && exit 1; }
fi

# Build the library
mkdir -p build && cd build || { echo "Failed to create build directory!" && exit 1; }

echo "Cmake Gemerating..."
cmake .. || { echo "Cmake Failed!" && exit 1; }

echo "Building..."
make -sj$(nproc) || { echo "Build Failed!" && exit 1; }

echo "Google Test Compiled Successfully!"