cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ENGINE_GLEW_INCLUDE_DIRS 			  "${PROJECT_SOURCE_DIR}/external/glew/include")
set(ENGINE_SDL2_INCLUDE_DIRS 			  "${PROJECT_SOURCE_DIR}/external/SDL2/include")
set(ENGINE_RAPIDJSON_INCLUDE_DIRS 		  "${PROJECT_SOURCE_DIR}/external/rapidjson")
set(ENGINE_IMGUI_INCLUDE_DIRS 			  "${PROJECT_SOURCE_DIR}/external/imgui")
set(ENGINE_GLM_INCLUDE_DIRS 			  "${PROJECT_SOURCE_DIR}/external/glm")
set(ENGINE_LIBZIP_INCLUDE_DIRS 			  "${PROJECT_SOURCE_DIR}/external/Libzip/lib")
set(ENGINE_SIMPLEFILEWATCHER_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/external/simplefilewatcher")
set(ENGINE_STB_INCLUDE_DIRS 	  		  "${PROJECT_SOURCE_DIR}/external/stb")
set(ENGINE_DELEGATES_INCLUDE_DIRS 		  "${PROJECT_SOURCE_DIR}/external/Delegates11")
set(ENGINE_BULLET_INCLUDE_DIRS    		  "${PROJECT_SOURCE_DIR}/external/Bullet 3/src")
set(ENGINE_LUAJIT_INCLUDE_DIRS    		  "${PROJECT_SOURCE_DIR}/external/luajit/src")
set(ENGINE_SOL2_INCLUDE_DIRS 	  		  "${PROJECT_SOURCE_DIR}/external/sol2")
set(ENGINE_TERMINUS_INCLUDE_DIRS  		  "${PROJECT_SOURCE_DIR}/engine/include")

file(GLOB_RECURSE TERMINUS_SOURCE *.cpp
						  		  *.h
						  		  *.c
						  		  ${PROJECT_SOURCE_DIR}/engine/include/*.h)

find_package(OpenGL REQUIRED)

add_library(Terminus-Engine ${TERMINUS_SOURCE})

set_target_properties( Terminus-Engine
    				   PROPERTIES
    				   ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/engine"
    				   LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/engine"
    				   RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/" )

target_include_directories(Terminus-Engine PUBLIC "${ENGINE_GLEW_INCLUDE_DIRS}"			 
												  "${ENGINE_RAPIDJSON_INCLUDE_DIRS}"
												  "${ENGINE_IMGUI_INCLUDE_DIRS}" 			 
												  "${ENGINE_GLM_INCLUDE_DIRS}" 			 
												  "${ENGINE_LIBZIP_INCLUDE_DIRS}" 			 
												  "${ENGINE_SIMPLEFILEWATCHER_INCLUDE_DIRS}"
												  "${ENGINE_STB_INCLUDE_DIRS}" 
												  "${ENGINE_DELEGATES_INCLUDE_DIRS}" 		 
												  "${ENGINE_BULLET_INCLUDE_DIRS}"    		 
												  "${ENGINE_LUAJIT_INCLUDE_DIRS}"    		 
												  "${ENGINE_SOL2_INCLUDE_DIRS}" 	  		 
												  "${ENGINE_TERMINUS_INCLUDE_DIRS}")

target_link_libraries(Terminus-Engine glew_s)
target_link_libraries(Terminus-Engine assimp)
target_link_libraries(Terminus-Engine imgui)
target_link_libraries(Terminus-Engine luajit-static)
target_link_libraries(Terminus-Engine zipstatic)
target_link_libraries(Terminus-Engine simplefilewatcher)
target_link_libraries(Terminus-Engine ${OPENGL_LIBRARIES})
target_link_libraries(Terminus-Engine Bullet2FileLoader)
target_link_libraries(Terminus-Engine Bullet3Collision)
target_link_libraries(Terminus-Engine Bullet3Common)
target_link_libraries(Terminus-Engine Bullet3Dynamics)
target_link_libraries(Terminus-Engine Bullet3Geometry)
target_link_libraries(Terminus-Engine Bullet3OpenCL_clew)
target_link_libraries(Terminus-Engine BulletCollision)
target_link_libraries(Terminus-Engine BulletDynamics)
target_link_libraries(Terminus-Engine BulletSoftBody)
target_link_libraries(Terminus-Engine BulletInverseDynamics)
target_link_libraries(Terminus-Engine LinearMath)
target_link_libraries(Terminus-Engine nfd)
target_link_libraries(Terminus-Engine Boxer)

IF(NOT WIN32)
    target_link_libraries(Terminus-Engine openal)
ELSE()
    target_link_libraries(Terminus-Engine OpenAL32)
ENDIF()