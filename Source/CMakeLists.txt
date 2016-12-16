cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project("Terminus")

set(T_GLEW_INCLUDE_DIRS "Third Party/glew/include")
set(T_SDL2_INCLUDE_DIRS "Third Party/SDL2/include")
set(T_RAPIDJSON_INCLUDE_DIRS "Third Party/rapidjson")
set(T_IMGUI_INCLUDE_DIRS "Third Party/imgui")
set(T_GLM_INCLUDE_DIRS "Third Party/glm")
set(T_LIBZIP_INCLUDE_DIRS "Third Party/Libzip/lib")
set(T_SIMPLEFILEWATCHER_INCLUDE_DIRS "Third Party/simplefilewatcher")
set(T_STB_INCLUDE_DIRS "Third Party/stb")
set(T_DELEGATES_INCLUDE_DIRS "Third Party/Delegates11")
set(T_BULLET_INCLUDE_DIRS "Third Party/Bullet 3/src")
set(T_LUAJIT_INCLUDE_DIRS "Third Party/luajit/src")

set(T_SOL2_INCLUDE_DIRS "Third Party/sol2")

set(T_TERMINUS_INCLUDE_DIRS "Terminus-Engine")

message("${T_GLEW_INCLUDE_DIRS}")
message("${T_SDL2_INCLUDE_DIRS}")
message("${T_RAPIDJSON_INCLUDE_DIRS}")
message("${T_IMGUI_INCLUDE_DIRS}")
message("${T_GLM_INCLUDE_DIRS}")
message("${T_LIBZIP_INCLUDE_DIRS}")
message("${T_SIMPLEFILEWATCHER_INCLUDE_DIRS}")
message("${T_STB_INCLUDE_DIRS}")
message("${T_DELEGATES_INCLUDE_DIRS}")
message("${T_BULLET_INCLUDE_DIRS}")
message("${T_LUAJIT_INCLUDE_DIRS}")
message("${T_SOL2_INCLUDE_DIRS}")
message("${T_TERMINUS_INCLUDE_DIRS}")

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
					"${T_TERMINUS_INCLUDE_DIRS}")

add_subdirectory("Terminus-Engine")
add_subdirectory("Terminus-Editor")
add_subdirectory("Third Party")