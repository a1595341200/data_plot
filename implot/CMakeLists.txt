cmake_minimum_required(VERSION 3.15)
set(PROJECT_N implot)
project(${PROJECT_N} LANGUAGES CXX)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

file(GLOB root_header_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB root_src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(${PROJECT_N} SHARED ${root_header_files} ${root_src_files})
target_link_libraries(${PROJECT_N} PUBLIC imgui)
