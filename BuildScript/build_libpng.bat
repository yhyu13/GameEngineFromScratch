@echo off
git submodule update --init External/src/libpng
mkdir External\build\libpng
pushd External\build\libpng
cmake -DCMAKE_INSTALL_PREFIX=../../Windows ../../src/libpng
cmake --build . --config Release --target install
popd

