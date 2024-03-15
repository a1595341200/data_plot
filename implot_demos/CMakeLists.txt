cmake_minimum_required(VERSION 3.15)
set(PROJECT_N app)
project(${PROJECT_N} LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/../../build_install)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build_install")

find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
# find_package(glfw3 CONFIG REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/nativefiledialog-extended)
file(GLOB root_header_files "${CMAKE_CURRENT_SOURCE_DIR}/common/*.h")
file(GLOB root_src_files "${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/common/Fonts/*.cpp")


add_library(${PROJECT_N} 
      ${root_src_files}
)

target_include_directories(${PROJECT_N}  
    PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
    $<INSTALL_INTERFACE:include/app>
)
target_link_libraries(${PROJECT_N}
    PUBLIC
    implot::implot
    nfd
    glad
)
target_compile_features(${PROJECT_N}  PRIVATE cxx_std_17)

install(TARGETS ${PROJECT_N} glad nfd
            EXPORT ${PROJECT_N} 
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
)
file(GLOB 3rdparty_header_files "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/*.hpp")

install(FILES
            ${root_header_files}
            ${3rdparty_header_files}
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/nativefiledialog-extended/src/include/nfd.h
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/nativefiledialog-extended/src/include/nfd.hpp
            DESTINATION include/app
)

install(DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad/include/glad
            ${CMAKE_CURRENT_SOURCE_DIR}/common/Fonts
            DESTINATION include/app
)

install(EXPORT
            ${PROJECT_N}
            NAMESPACE  "app::"
            DESTINATION  lib/cmake/app
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/cmake/appConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/appConfig.cmake
        INSTALL_DESTINATION lib/cmake
)

install(FILES 
        ${CMAKE_CURRENT_BINARY_DIR}/appConfig.cmake DESTINATION lib/cmake/app)


