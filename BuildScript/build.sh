#!/bin/bash
mkdir build
cd build
cmake ..
cmake --build . --config debug
ctest -C Debug -V
