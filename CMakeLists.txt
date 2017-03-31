cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("Terminus")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/external")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/external")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

IF(APPLE)
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	set(CMAKE_EXE_LINKER_FLAGS "-pagezero_size 10000 -image_base 100000000")
ENDIF()

set(T_GLEW_INCLUDE_DIRS 			  "${PROJECT_SOURCE_DIR}/external/glew/include")
set(T_SDL2_INCLUDE_DIRS 			  "${PROJECT_SOURCE_DIR}/external/SDL2/include")
set(T_RAPIDJSON_INCLUDE_DIRS 		  "${PROJECT_SOURCE_DIR}/external/rapidjson")
set(T_IMGUI_INCLUDE_DIRS 			  "${PROJECT_SOURCE_DIR}/external/imgui")
set(T_GLM_INCLUDE_DIRS 			  	  "${PROJECT_SOURCE_DIR}/external/glm")
set(T_LIBZIP_INCLUDE_DIRS 			  "${PROJECT_SOURCE_DIR}/external/Libzip/lib")
set(T_SIMPLEFILEWATCHER_INCLUDE_DIRS  "${PROJECT_SOURCE_DIR}/external/simplefilewatcher")
set(T_STB_INCLUDE_DIRS 	  		  	  "${PROJECT_SOURCE_DIR}/external/stb")
set(T_DELEGATES_INCLUDE_DIRS 		  "${PROJECT_SOURCE_DIR}/external/Delegates11")
set(T_BULLET_INCLUDE_DIRS    		  "${PROJECT_SOURCE_DIR}/external/Bullet 3/src")
set(T_LUAJIT_INCLUDE_DIRS    		  "${PROJECT_SOURCE_DIR}/external/luajit/src")
set(T_SOL2_INCLUDE_DIRS 	  		  "${PROJECT_SOURCE_DIR}/external/sol2")
set(T_CMFT_INCLUDE_DIRS 	  		  "${PROJECT_SOURCE_DIR}/external/cmft/include/cmft")
set(T_TERMINUS_INCLUDE_DIRS  		  "${PROJECT_SOURCE_DIR}/engine/include")

include_directories("${T_GLEW_INCLUDE_DIRS}" 
					"${T_SDL2_INCLUDE_DIRS}"
					"${T_RAPIDJSON_INCLUDE_DIRS}"
					"${T_IMGUI_INCLUDE_DIRS}"
					"${T_GLM_INCLUDE_DIRS}"
					"${T_LIBZIP_INCLUDE_DIRS}"
					"${T_SIMPLEFILEWATCHER_INCLUDE_DIRS}"
					"${T_STB_INCLUDE_DIRS}"
					"${T_DELEGATES_INCLUDE_DIRS}"
					"${T_BULLET_INCLUDE_DIRS}"
					"${T_LUAJIT_INCLUDE_DIRS}"
					"${T_SOL2_INCLUDE_DIRS}"
					"${T_CMFT_INCLUDE_DIRS}"
					"${T_TERMINUS_INCLUDE_DIRS}")

add_subdirectory(external)
add_subdirectory(engine/src)
add_subdirectory(runtime)
add_subdirectory(editor)

