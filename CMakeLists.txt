cmake_minimum_required(VERSION 3.26)
project(constellation)

set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(MyQtApp
        main.cpp
        mainwindow.cpp
        graph.cpp
        graph.h
)

target_link_libraries(MyQtApp Qt5::Widgets)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
