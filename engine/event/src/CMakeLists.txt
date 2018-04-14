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
set(TE_EVENT_PUBLIC_HEADERS "${PROJECT_SOURCE_DIR}/include/event_manager.hpp"
                            "${PROJECT_SOURCE_DIR}/include/event.hpp"
                            "${PROJECT_SOURCE_DIR}/include/event_macros.hpp")

# Private Headers
set(TE_EVENT_PRIVATE_HEADERS "${PROJECT_SOURCE_DIR}/src/event_manager_impl.hpp")

# Sources
set(TE_EVENT_SOURCES "${PROJECT_SOURCE_DIR}/src/event_manager_impl.cpp")

# Source groups
source_group("public" FILES ${TE_EVENT_PUBLIC_HEADERS})
source_group("private" FILES ${TE_EVENT_PRIVATE_HEADERS})
source_group("sources" FILES ${TE_EVENT_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

add_library(teEvent ${TE_EVENT_PUBLIC_HEADERS} ${TE_EVENT_PRIVATE_HEADERS} ${TE_EVENT_SOURCES})
set_target_properties(teEvent PROPERTIES FOLDER "engine")