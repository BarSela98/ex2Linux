#!/bin/bash

mkdir cmake-build-debug
mv fetchStockData.sh cmake-build-debug
cd cmake-build-debug
cmake ..
make
./linux2


