cmake_minimum_required(VERSION 3.20)
project(tiny-rasterizer)

set (CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE RASTERIZER_HEADERS "include/*.hpp")
file(GLOB_RECURSE RASTERIZER_SOURCES "source/*.cpp")

add_executable(tiny-rasterizer
    ${RASTERIZER_HEADERS}
    ${RASTERIZER_SOURCES}
)

target_include_directories(tiny-rasterizer PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${SDL2_INCLUDE_DIRS}"
)

target_link_libraries(tiny-rasterizer PUBLIC
    ${SDL2_LIBRARIES}
)
target_compile_definitions(tiny-rasterizer PUBLIC -DPROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}")
