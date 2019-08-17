mkdir build
pushd build
cmake ..
cmake --build . --config Debug
ctest -C Debug -V
popd

