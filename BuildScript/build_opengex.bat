@echo off
git submodule update --init External/src/opengex
mkdir External\build\opengex
pushd External\build\opengex
cmake -DCMAKE_INSTALL_PREFIX=../../ ../../src/opengex
cmake --build . --config Release --target install
popd

