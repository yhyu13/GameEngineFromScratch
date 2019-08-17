#!/bin/bash
git submodule update --init External/src/libpng
mkdir External/build/libpng
cd External/build/libpng
cmake -DCMAKE_INSTALL_PREFIX=../../Linux ../../src/libpng
cmake --build . --config Debug --target install

