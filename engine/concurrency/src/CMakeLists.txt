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

# Headers
set(TE_CONCURRENCY_HEADERS "${PROJECT_SOURCE_DIR}/include/lock.hpp"
                           "${PROJECT_SOURCE_DIR}/include/mutex.hpp"
                           "${PROJECT_SOURCE_DIR}/include/scoped_lock.hpp"
                           "${PROJECT_SOURCE_DIR}/include/semaphore.hpp"
                           "${PROJECT_SOURCE_DIR}/include/thread_common.hpp"
                           "${PROJECT_SOURCE_DIR}/include/thread.hpp")

# Sources
set(TE_CONCURRENCY_SOURCES "lock.cpp"
                           "mutex.cpp"
                           "scoped_lock.cpp"
                           "semaphore.cpp"
                           "thread.cpp")

# Source groups
source_group("Headers" FILES ${TE_CONCURRENCY_HEADERS})
source_group("Sources" FILES ${TE_CONCURRENCY_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

add_library(teConcurrency ${TE_CONCURRENCY_HEADERS} ${TE_CONCURRENCY_SOURCES})
set_target_properties(teConcurrency PROPERTIES FOLDER "engine")