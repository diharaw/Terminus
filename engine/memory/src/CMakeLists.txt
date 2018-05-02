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
set(TE_MEMORY_PUBLIC_HEADERS "${PROJECT_SOURCE_DIR}/include/allocator.hpp"
                             "${PROJECT_SOURCE_DIR}/include/memory_macros.hpp")

# Private Headers
set(TE_MEMORY_PRIVATE_HEADERS "${PROJECT_SOURCE_DIR}/src/buffer_linear_allocator.hpp"
                              "${PROJECT_SOURCE_DIR}/src/buffer_pool_allocator.hpp"
                              "${PROJECT_SOURCE_DIR}/src/buffer_stack_allocator.hpp"
                              "${PROJECT_SOURCE_DIR}/src/heap_allocator.hpp"
                              "${PROJECT_SOURCE_DIR}/src/linear_allocator.hpp"
                              "${PROJECT_SOURCE_DIR}/src/pool_allocator.hpp")

# Sources
set(TE_MEMORY_SOURCES "heap_allocator.cpp"
                      "linear_allocator.cpp"
                      "pool_allocator.cpp")

# Source groups
source_group("public" FILES ${TE_MEMORY_PUBLIC_HEADERS})
source_group("private" FILES ${TE_MEMORY_PRIVATE_HEADERS})
source_group("sources" FILES ${TE_MEMORY_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

add_library(teMemory ${TE_MEMORY_PUBLIC_HEADERS} ${TE_MEMORY_PRIVATE_HEADERS} ${TE_MEMORY_SOURCES})
set_target_properties(teMemory PROPERTIES FOLDER "engine")