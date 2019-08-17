@echo off
git submodule update --init External/src/libpng
mkdir -p External\build\libpng
pushd External\build\libpng
cmake -DCMAKE_INSTALL_PREFIX=../../Windows -G "NMake Makefiles" ../../src/libpng
cmake --build . --config release --target install
popd

