#!/bin/bash

set -e

# If the script is executed from the scripts folder then
# change one directory up.
if [ "$(basename $PWD)" ==  "scripts" ]; then
	echo "Changing to project root"
	cd ..
fi

# Check that the scripts cwd is the actual project directory.
if [ ! -f "$PWD/goplugin.h" ]; then
	echo "Script executed from the wrong directory"
	exit 1
fi

# Check that the working directory is clean.
if [ ! -z "$(git status --untracked-files=no --porcelain)" ]; then
	echo "Working directory not clean! Commit your code changes before applying style changes"
	exit 1
fi

# Check that the clang-format executable exists.
if ! command -v clang-format > /dev/null; then
	echo "clang-format not found. Make sure it's installed."
	exit 1
fi

# Run clang-format
echo "Running clang-format"
clang-format -style=file -i ./**/*.{h,cpp}
