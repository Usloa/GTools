cmake_minimum_required(VERSION 3.21)
project(GTools VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

add_executable(GTools WIN32
    ${SOURCES}
    resources/gtools.qrc
)

target_include_directories(GTools PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(GTools PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    user32
    gdi32
    dwmapi
)
