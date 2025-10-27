cmake_minimum_required(VERSION 3.10)

project(CPURayTracer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(third-party/glm)
add_subdirectory(third-party/SFML)
add_subdirectory(third-party/rapidobj)
add_subdirectory(third-party/openexr)

if(MSVC)
    target_compile_options(OpenEXRCore PRIVATE /arch:AVX2)
    target_compile_options(OpenEXR PRIVATE /arch:AVX2)
else()
    target_compile_options(OpenEXRCore PRIVATE -msse4.1)
    target_compile_options(OpenEXR PRIVATE -msse4.1)
endif()

add_subdirectory(source)
