cmake_minimum_required(VERSION 3.16)
project(svg-animator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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

# Platform-specific configurations
if(WIN32)
    # Windows: Set subsystem to Windows (no console window)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
    # Add Windows-specific defines
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        UNICODE
        _UNICODE
    )
elseif(APPLE)
    # macOS: Create app bundle
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "SVG Animator"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.svganimator.app"
    )
    # Add macOS-specific flags
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wno-deprecated")
else()
    # Linux/Unix: Install to standard locations
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        BUNDLE DESTINATION bin
    )
endif()

# Set compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
