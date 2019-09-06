git submodule update --init External/src/llvm External/src/clang
mkdir -p External\build\llvm
cd External\build\llvm
cmake -DCMAKE_INSTALL_PREFIX=../../Windows -DLLVM_ENABLE_PROJECTS=clang -G "Visual Studio 16 2019" thost=x64 ../../src/llvm
cmake --build . --config release --target install

