cmake_minimum_required(VERSION 3.15)
set(PROJECT_N imguizmo)
project(${PROJECT_N} LANGUAGES CXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/../../../build_install)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build_install")
find_package(imgui CONFIG REQUIRED)
# find_package(glfw3 CONFIG REQUIRED)
if(imgui_FOUND)
    message(STATUS "imgui found")
else()
    message(STATUS "imgui not found")
endif()
file(GLOB root_header_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB root_src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(${PROJECT_N} ${root_header_files} ${root_src_files})
# add_library(${PROJECT_N} SHARED ${root_header_files} ${root_src_files})
# add_executable(demo ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-example/main.cpp)

add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

# target_include_directories(demo
#     PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}o/vcpkg-example/ImApp.h
# )

# target_link_libraries(demo PUBLIC imgui::imgui)

target_include_directories(${PROJECT_N}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/imguizmo>
)

target_link_libraries(${PROJECT_N} 
    PUBLIC 
    imgui::imgui 
    # glfw
)

install(TARGETS ${PROJECT_N} 
            EXPORT imguizmo
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
)

install(FILES
            ${root_header_files} 
            DESTINATION include/imguizmo
)

install(EXPORT
            imguizmo
            NAMESPACE  "imguizmo::"
            DESTINATION  lib/cmake/imguizmo
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/cmake/imguizmoConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/imguizmoConfig.cmake
        INSTALL_DESTINATION lib/cmake
)

install(FILES 
        ${CMAKE_CURRENT_BINARY_DIR}/imguizmoConfig.cmake DESTINATION lib/cmake/imguizmo)

