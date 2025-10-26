file(GLOB_RECURSE source CONFIGURE_DEPENDS *.cpp)

add_executable(CPURayTracer ${source})

target_include_directories(CPURayTracer PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_precompile_headers(CPURayTracer
    PRIVATE
        <any>
        <list>
        <span>
        <tuple>
        <regex>
        <ranges>
        <atomic>
        <string>
        <vector>
        <random>
        <chrono>
        <thread>
        <future>
        <variant>
        <sstream>
        <fstream>
        <filesystem>
        <iostream>
        <optional>
        <exception>
        <execution>
        <unordered_map>
        <array>
        <algorithm>
        <cassert>
        <cfloat>
        <charconv>
        <cmath>
        <cstring>
        <map>
        <system_error>
        <utility>
        <limits>

        <glm/glm.hpp>
)
target_compile_definitions(
    CPURayTracer
        PUBLIC
            NOMINMAX
            WIN32_LEAN_AND_MEAN
            GLM_FORCE_DEPTH_ZERO_TO_ONE
            GLM_FORCE_RADIANS
            GLM_FORCE_LEFT_HANDED
)
target_compile_definitions(CPURayTracer PUBLIC WITH_DEBUG_INFO)
target_link_libraries(CPURayTracer glm rapidobj sfml-graphics OpenEXR::OpenEXR)
