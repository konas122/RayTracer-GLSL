cmake_minimum_required(VERSION 3.10)

project(CPURayTracer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(third-party/glm)
add_subdirectory(third-party/rapidobj)

add_subdirectory(source)
