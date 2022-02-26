project(Lemonade VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)

set(Lemonade_SRC
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/mainwindow.ui
    src/openglwidget.h
    src/openglwidget.cpp
)

add_executable(lemonade ${Lemonade_SRC})
target_include_directories(lemonade PRIVATE src/)


target_link_libraries(lemonade PUBLIC
    lemon::lemon
    Qt6::Widgets
    Qt6::OpenGLWidgets
)
