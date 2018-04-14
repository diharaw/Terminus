cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# -----------------------------------------------------------------------------------------------------------------------------
# FLAGS
# -----------------------------------------------------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")

# -----------------------------------------------------------------------------------------------------------------------------
# DEFINITIONS
# -----------------------------------------------------------------------------------------------------------------------------

add_definitions(-DTE_PLATFORM_SDL2)

# -----------------------------------------------------------------------------------------------------------------------------
# FILES
# -----------------------------------------------------------------------------------------------------------------------------

# Public Headers
set(TE_PLATFORM_PUBLIC_HEADERS "${PROJECT_SOURCE_DIR}/include/platform_macros.hpp"
                               "${PROJECT_SOURCE_DIR}/include/window.hpp")

# Private Headers
set(TE_PLATFORM_PRIVATE_HEADERS "${PROJECT_SOURCE_DIR}/src/window_sdl2.hpp")

# Sources
set(TE_PLATFORM_SOURCES "window_sdl2.cpp")

# Source groups
source_group("public" FILES ${TE_PLATFORM_PUBLIC_HEADERS})
source_group("private" FILES ${TE_PLATFORM_PRIVATE_HEADERS})
source_group("sources" FILES ${TE_PLATFORM_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

add_library(tePlatform ${TE_PLATFORM_PUBLIC_HEADERS} ${TE_PLATFORM_PRIVATE_HEADERS} ${TE_PLATFORM_SOURCES})
set_target_properties(tePlatform PROPERTIES FOLDER "engine")