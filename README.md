# GameEngineFromScratch
Game Engine From Scratch

# Build

```mkdir build; cd build; cmake ..; cmake --build . --config Debug --clean-first; ctest -C Debug -V```

Or

```./BuildScript/build.sh``` on Linux, ```./BuildScript/build.bat``` on Windows, 

# Documentation

```doxygen ./Doxyfile``` to generate the web documentation for this project under the ```./Document```.