cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE TERMINUS_EDITOR_SOURCE *.cpp
						  		  		 *.h)

add_executable(Terminus-Editor ${TERMINUS_EDITOR_SOURCE})

set_target_properties( Terminus-Editor
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/Terminus"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/Terminus"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
target_include_directories(Terminus-Editor PUBLIC "../Terminus-Engine")

target_link_libraries(Terminus-Editor Terminus-Engine)
