cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ENGINE_TERMINUS_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/engine/include")
set(ENGINE_SDL2_INCLUDE_DIRS 	 "${PROJECT_SOURCE_DIR}/external/SDL2/include")

file(GLOB_RECURSE RUNTIME_SOURCE  *.cpp
						  		  *.h
						  		  *.c)

add_executable(Terminus-Runtime ${RUNTIME_SOURCE})

set_target_properties( Terminus-Runtime
    				   PROPERTIES
    				   ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/runtime"
    				   LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/runtime"
    				   RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/runtime" )

IF(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
    set_target_properties(Terminus-Runtime PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(Terminus-Runtime PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
  endif()
ENDIF()

target_include_directories(Terminus-Runtime PUBLIC "${ENGINE_TERMINUS_INCLUDE_DIRS}"
												   "${ENGINE_SDL2_INCLUDE_DIRS}")

target_link_libraries(Terminus-Runtime SDL2-static)
target_link_libraries(Terminus-Runtime Terminus-Engine)