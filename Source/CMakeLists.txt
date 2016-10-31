cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE TERMINUS_SOURCE Terminus/*.cpp
						  		  Terminus/*.h)

project("Terminus")

add_subdirectory("Third Party")

include_directories("Third Party/glew/include”)
include_directories("Third Party/glfw/include”)
include_directories("Third Party/rapidjson")
include_directories("Third Party/imgui")
include_directories("Third Party/glm")
include_directories("Third Party/Libzip/lib")
include_directories("Third Party/simplefilewatcher")
include_directories("Third Party/stb")
include_directories("Third Party/FastDelegate")
include_directories("Third Party/Delegates11”)
include_directories("Third Party/Bullet 3/src")

find_package(OpenGL REQUIRED)

add_executable(Terminus ${TERMINUS_SOURCE})

set_target_properties( Terminus
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/Terminus"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/Terminus"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(Terminus glfw)
target_link_libraries(Terminus glew_s)
target_link_libraries(Terminus assimp)
target_link_libraries(Terminus imgui)
target_link_libraries(Terminus zipstatic)
target_link_libraries(Terminus angelscript)
target_link_libraries(Terminus simplefilewatcher)
target_link_libraries(Terminus ${OPENGL_LIBRARIES})
target_link_libraries(Terminus Bullet2FileLoader)
target_link_libraries(Terminus Bullet3Collision)
target_link_libraries(Terminus Bullet3Common)
target_link_libraries(Terminus Bullet3Dynamics)
target_link_libraries(Terminus Bullet3Geometry)
target_link_libraries(Terminus Bullet3OpenCL_clew)
target_link_libraries(Terminus BulletCollision)
target_link_libraries(Terminus BulletDynamics)
target_link_libraries(Terminus BulletSoftBody)
target_link_libraries(Terminus BulletInverseDynamics)
target_link_libraries(Terminus LinearMath)
target_link_libraries(Terminus nfd)
target_link_libraries(Terminus Boxer)

IF(NOT WIN32)
    target_link_libraries(Terminus openal)
ELSE()
    target_link_libraries(Terminus OpenAL32)
ENDIF()