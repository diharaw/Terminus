cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


file(GLOB_RECURSE EDITOR_SOURCE  *.cpp
						  		  *.h
						  		  *.c)

add_executable(Terminus-Editor ${EDITOR_SOURCE})

set_target_properties( Terminus-Editor
    				   PROPERTIES
    				   ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/editor"
    				   LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/editor"
    				   RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/editor" )

target_link_libraries(Terminus-Editor Terminus-Engine)