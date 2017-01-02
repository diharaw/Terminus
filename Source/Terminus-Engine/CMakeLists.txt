cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE TERMINUS_SOURCE *.cpp
						  		  *.h
						  		  *.c)

find_package(OpenGL REQUIRED)

add_library(Terminus-Engine ${TERMINUS_SOURCE})

set_target_properties( Terminus-Engine
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/Terminus"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/Terminus"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(Terminus-Engine SDL2-static)
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