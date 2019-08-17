#!/bin/bash
git submodule update --init External/src/opengex
mkdir External/build/opengex
cd External/build/opengex
cmake -DCMAKE_INSTALL_PREFIX=../../ ../../src/opengex
cmake --build . --config Debug --target install

