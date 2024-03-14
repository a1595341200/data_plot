cmake_minimum_required(VERSION 3.15)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/implot
    ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/common
    ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/3rdparty
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
add_subdirectory(imgui)
add_subdirectory(implot)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/3rdparty/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/3rdparty/nativefiledialog-extended)

add_executable(plotdemo 
    main.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/common/App.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/common/Fonts/Fonts.h
    ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/common/Fonts/FontAwesome5.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/common/Fonts/RobotoRegular.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/common/Fonts/RobotoBold.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/common/Fonts/RobotoItalic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/common/Fonts/RobotoMonoRegular.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/common/Fonts/RobotoMonoBold.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/implot_demos/common/Fonts/RobotoMonoItalic.cpp

)
target_link_libraries(plotdemo implot glad nfd)
