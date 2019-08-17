@echo off
git submodule update --init External/src/zlib
mkdir -p External\build\zlib
pushd External\build\zlib
cmake -DCMAKE_INSTALL_PREFIX=../../ -G "NMake Makefiles" ../../src/zlib
cmake --build . --config Release --target install
popd

