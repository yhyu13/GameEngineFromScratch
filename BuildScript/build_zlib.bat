@echo off
git submodule update --init External/src/zlib
mkdir External\build\zlib
pushd External\build\zlib
cmake -DCMAKE_INSTALL_PREFIX=../../ ../../src/zlib
cmake --build . --config Release --target install
popd

