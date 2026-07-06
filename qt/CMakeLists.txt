cmake_minimum_required(VERSION 3.16)
project(svg-animator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Svg Xml)

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/SvgView.cpp
    src/ElementPanel.cpp
    src/AnimationEngine.cpp
    src/WorkspaceManager.cpp
    src/TrajectoryManager.cpp
    src/SlideshowManager.cpp
    src/ExportManager.cpp
    src/Shapes.cpp
    src/ControlsWidget.cpp
    src/Theme.cpp
    src/BackgroundImageManager.cpp
)

set(HEADERS
    src/MainWindow.h
    src/SvgView.h
    src/ElementPanel.h
    src/AnimationEngine.h
    src/WorkspaceManager.h
    src/TrajectoryManager.h
    src/SlideshowManager.h
    src/ExportManager.h
    src/Shapes.h
    src/ControlsWidget.h
    src/Theme.h
    src/BackgroundImageManager.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Svg
    Qt5::Xml
)

target_include_directories(${PROJECT_NAME} PRIVATE src)
