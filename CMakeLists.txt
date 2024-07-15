cmake_minimum_required(VERSION 3.15)
project(plotdemo)
# set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/../../build_install)
# list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build_install")
# include_directories(
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
#     ${CMAKE_CURRENT_SOURCE_DIR}/implot
#     ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/common
#     ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/3rdparty
# )
set(CMAKE_CXX_FLAGS "-g3")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
find_package(app CONFIG REQUIRED)
# find_package(glfw3 CONFIG REQUIRED)
add_compile_options(
    -Wall            # 启用所有常见警告
    -Wextra          # 启用额外的警告
    -Werror          # 将警告视为错误
    -pedantic        # 严格遵循C标准
    -Wshadow         # 警告变量名被隐藏的情况
    -Wno-conversion  # 忽略隐式转换警告
    -Wsign-conversion # 警告有符号和无符号之间的隐式转换
    -Wno-unused      # 忽略未使用警告
)
add_executable(plotdemo 
    plotdemo.cpp 
)
target_link_libraries(plotdemo PUBLIC app::app ${CMAKE_DL_LIBS})
target_compile_features(plotdemo PRIVATE cxx_std_17)

include_directories(
    include
)

add_executable(plotdemo1
    plotdemo1.cpp 
    src/ObjectData.cpp
    src/ObjectPlot.cpp
    src/AppLog.cpp
)
target_link_libraries(plotdemo1 PUBLIC app::app ${CMAKE_DL_LIBS} pthread)
target_compile_features(plotdemo1 PRIVATE cxx_std_17)
