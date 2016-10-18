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