add_executable(AssetLoaderTest AssetLoaderTest.cpp)
add_executable(AllocatorTest AllocatorTest.cpp)

target_link_libraries(AssetLoaderTest Common Utility)
target_link_libraries(AllocatorTest Common GeomMath Utility)

add_test(NAME TEST_AssetLoader COMMAND AssetLoaderTest)
add_test(NAME TEST_Allocator COMMAND AllocatorTest)




