cmake_minimum_required(VERSION 3.15)
set(PROJECT_N imgui)
project(${PROJECT_N} LANGUAGES CXX)
# include(FetchContent)
# message("FetchContent glfw")
# FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw) 
# FetchContent_MakeAvailable(glfw)

find_package(glfw3 CONFIG REQUIRED)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED  TRUE)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/../../build_install)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

file(GLOB root_header_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB root_src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

file(GLOB imgui_header_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB imgui_src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

file(GLOB backends_header_files "${CMAKE_CURRENT_SOURCE_DIR}/backends/*.h")
# message(STATUS "backends_header_files: ${backends_header_files}")
# file(GLOB backends_src_files "${CMAKE_CURRENT_SOURCE_DIR}/backends/*.cpp")
list(APPEND backends_src_files 
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)

list(APPEND PRJ_LIBRARIES ${GLFW_LIB})
list(APPEND PRJ_LIBRARIES ${OPENGL_LIBRARIES})

add_library(${PROJECT_N} ${imgui_src_files} ${backends_src_files})
# add_library(${PROJECT_N} SHARED ${imgui_src_files} ${backends_src_files})

target_include_directories(${PROJECT_N}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/backends>
        $<BUILD_INTERFACE:${GLFW_HOME}/include>
        $<INSTALL_INTERFACE:include/imgui>
)

target_link_libraries(${PROJECT_N}
        PUBLIC
        ${PRJ_LIBRARIES}
        # ${GLFW_LIBRARIES} glfw GL
        glfw
)

install(TARGETS ${PROJECT_N} 
            EXPORT imgui
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
)

install(FILES
                ${root_header_files}
                ${backends_header_files}
                DESTINATION include/imgui
)

install(EXPORT
                imgui
                NAMESPACE  "imgui::"
                DESTINATION  lib/cmake/imgui
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/cmake/imguiConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/imguiConfig.cmake
        INSTALL_DESTINATION lib/cmake
)

install(FILES 
        ${CMAKE_CURRENT_BINARY_DIR}/imguiConfig.cmake DESTINATION lib/cmake/imgui)
# 其他人使用的时候设置查找路径
#set(CMAKE_PREFIX_PATH "/home/hupeng/code_c/github/use_cmake/course_07/make_lib/install")
set(BUILD_DEMO ON)
if(${BUILD_DEMO})
        add_executable(demo ${CMAKE_CURRENT_SOURCE_DIR}/examples/example_glfw_opengl3/main.cpp)
        # target_link_libraries(demo PUBLIC imgui ${GLFW_LIBRARIES} glfw GL)
        if (APPLE)
                target_link_libraries(demo PUBLIC 
                        imgui 
                        ${GLFW_LIBRARIES} 
                        "-framework OpenGL"
                )
        else()
                target_link_libraries(demo PUBLIC 
                        imgui 
                        ${GLFW_LIBRARIES} 
                        GL
                )
        endif()

        target_include_directories(demo
                PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/backends>
                $<BUILD_INTERFACE:${GLFW_HOME}/include>
                $<INSTALL_INTERFACE:include/imgui>
        )
endif()
