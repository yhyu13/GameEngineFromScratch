add_executable(GameEngineFromScratchD3D12 src/WindowsApplication.cpp src/D3d12Application.cpp)
target_link_libraries(GameEngineFromScratchD3D12 Common D3dRHI d3d12 dxgi d3dcompiler)

add_executable(GameEngineFromScratchOpenGL 
    src/WindowsApplication.cpp 
    src/OpenGLApplication.cpp
    ${PROJECT_SOURCE_DIR}/External/GL/src/glad_wgl.c
    )
target_link_libraries(GameEngineFromScratchOpenGL Common OpenGLRHI opengl32)
target_include_directories(GameEngineFromScratchOpenGL PRIVATE "${PROJECT_SOURCE_DIR}/External/GL/include")
