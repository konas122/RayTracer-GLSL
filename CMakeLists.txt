cmake_minimum_required(VERSION 3.10)

project(main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

file(COPY resources/ DESTINATION ${CMAKE_BINARY_DIR}/Debug)
file(COPY source/shaders DESTINATION ${CMAKE_BINARY_DIR}/Debug)

set(SOURCE_DIR "source")
message("SOURCE_DIR: ${SOURCE_DIR}")
include_directories(
    ${PROJECT_SOURCE_DIR}/source
    ${PROJECT_SOURCE_DIR}/third-party
    ${PROJECT_SOURCE_DIR}/third-party/imgui
)

file(GLOB_RECURSE SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/source/*.cpp"
    "${PROJECT_SOURCE_DIR}/third-party/*.cpp"
    "${PROJECT_SOURCE_DIR}/third-party/imgui/*.cpp"
)

add_executable(
    main
    ${SOURCE_FILES}
)


find_package(glad CONFIG REQUIRED)
target_link_libraries(main PRIVATE glad::glad)
if(glad_FOUND)
    message("glad found")
endif()

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)
if(glfw3_FOUND)
    message("glfw3 found")
endif()

find_package(glm CONFIG REQUIRED)
target_link_libraries(main PRIVATE glm::glm)
if(glm_FOUND)
    message("glm found")
endif()
