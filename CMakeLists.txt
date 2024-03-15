cmake_minimum_required(VERSION 3.15)
project(plotdemo)
# set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/../../build_install)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build_install")
# include_directories(
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
#     ${CMAKE_CURRENT_SOURCE_DIR}/implot
#     ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/common
#     ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/3rdparty
# )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
find_package(app CONFIG REQUIRED)
# find_package(glfw3 CONFIG REQUIRED)

add_executable(plotdemo 
    main.cpp 
)
target_link_libraries(plotdemo PUBLIC app::app)
target_compile_features(plotdemo PRIVATE cxx_std_17)
