#!/bin/bash

# instructions
# ==================
# $ chmod +x setup.sh
# $ ./setup.sh
# ==================

rm -rf build
mkdir build
cd build
cmake ..
make

echo "starting server..."

./server