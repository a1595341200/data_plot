cmake_minimum_required(VERSION 3.15)
set(PROJECT_N imgui)
project(${PROJECT_N} LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(PRJ_LIBRARIES)
set(IMGUI_HOME ${CMAKE_CURRENT_SOURCE_DIR})
set(GLFW_INC ${GLFW_HOME}/include)
set(IMGUI_INC "${IMGUI_HOME}")
set(IMGUI_BACKEND "${IMGUI_HOME}/backends")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

file(GLOB root_header_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
message(${root_header_files})
file(GLOB root_src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

file(GLOB imgui_header_files "${IMGUI_INC}/*.h")
file(GLOB imgui_src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

file(GLOB backends_header_files "${CMAKE_CURRENT_SOURCE_DIR}/${IMGUI_BACKEND}/*.h")
# file(GLOB backends_src_files "${CMAKE_CURRENT_SOURCE_DIR}/backends/*.cpp")
list(APPEND backends_src_files 
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)
message(${backends_src_files})

list(APPEND PRJ_LIBRARIES ${GLFW_LIB})
list(APPEND PRJ_LIBRARIES ${OPENGL_LIBRARIES})

add_library(${PROJECT_N} SHARED ${imgui_src_files} ${backends_src_files})

add_executable(demo ${CMAKE_CURRENT_SOURCE_DIR}/examples/example_glfw_opengl3/main.cpp)
target_link_libraries(demo PUBLIC imgui ${GLFW_LIBRARIES} glfw GL)

message(${imgui_header_files})

target_include_directories(${PROJECT_N}
        PUBLIC
        ${PRJ_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_N}
        PUBLIC
        ${PRJ_LIBRARIES}
        ${GLFW_LIBRARIES} glfw GL
)
