#!/bin/bash

# instructions
# ==================
# $ chmod +x run.sh
# $ ./run.sh [name]
# ==================
# [name] is the name of the file without the .cpp extension

mkdir -p programs

g++ -o programs/$1 $1.cpp

./programs/$1