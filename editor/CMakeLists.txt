cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

IF(WIN32)
	set (CMAKE_PREFIX_PATH "C:\\Qt\\5.8\\msvc2015_64")
ENDIF()

IF(APPLE)
	set (CMAKE_PREFIX_PATH "/Users/diharaw/Qt/5.8/clang_64/lib/cmake")
ENDIF()

file(GLOB_RECURSE EDITOR_SOURCE  *.cpp
						  		  *.h
						  		  *.c)

set(EDITOR_UI_SOURCES mainwindow.ui)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_wrap_ui(UI_GENERATED_HEADERS ${EDITOR_UI_SOURCES})

add_executable(Terminus-Editor ${EDITOR_SOURCE} ${UI_GENERATED_HEADERS} style.qrc)

set_target_properties( Terminus-Editor
    				   PROPERTIES
    				   ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/editor"
    				   LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/editor"
    				   RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/editor" )

target_link_libraries(Terminus-Editor Qt5::Core)
target_link_libraries(Terminus-Editor Qt5::Gui)
target_link_libraries(Terminus-Editor Qt5::Widgets)
target_link_libraries(Terminus-Editor Terminus-Engine)