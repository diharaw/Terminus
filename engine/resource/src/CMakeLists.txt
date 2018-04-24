cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# -----------------------------------------------------------------------------------------------------------------------------
# FLAGS
# -----------------------------------------------------------------------------------------------------------------------------

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
endif()

# -----------------------------------------------------------------------------------------------------------------------------
# DEFINITIONS
# -----------------------------------------------------------------------------------------------------------------------------

add_definitions(-DTE_PLATFORM_SDL2)

# -----------------------------------------------------------------------------------------------------------------------------
# FILES
# -----------------------------------------------------------------------------------------------------------------------------

# Public Headers
set(TE_RESOURCE_PUBLIC_HEADERS "${PROJECT_SOURCE_DIR}/include/resource_manager.hpp")

# Private Headers
set(TE_RESOURCE_PRIVATE_HEADERS "${PROJECT_SOURCE_DIR}/src/resource_factory.hpp"
                                "${PROJECT_SOURCE_DIR}/src/resource_cache.hpp")

# Sources
set(TE_RESOURCE_SOURCES "resource_manager.cpp")

# Source groups
source_group("public" FILES ${TE_RESOURCE_PUBLIC_HEADERS})
source_group("private" FILES ${TE_RESOURCE_PRIVATE_HEADERS})
source_group("sources" FILES ${TE_RESOURCE_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

add_library(teResource ${TE_RESOURCE_PUBLIC_HEADERS} ${TE_RESOURCE_PRIVATE_HEADERS} ${TE_RESOURCE_SOURCES})
set_target_properties(teResource PROPERTIES FOLDER "engine")