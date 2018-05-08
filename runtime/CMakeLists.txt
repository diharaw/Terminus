cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# -----------------------------------------------------------------------------------------------------------------------------
# FLAGS
# -----------------------------------------------------------------------------------------------------------------------------

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
endif()

# -----------------------------------------------------------------------------------------------------------------------------
# FILES
# -----------------------------------------------------------------------------------------------------------------------------

# Sources
set(TE_RUNTIME_SOURCES "main.cpp")

# Source groups
source_group("src" FILES ${TE_RUNTIME_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

if(APPLE)
    add_executable(TerminusRuntime MACOSX_BUNDLE ${TE_RUNTIME_SOURCES})
elseif(WIN32)
    add_executable(TerminusRuntime WIN32 ${TE_RUNTIME_SOURCES})
else()
    add_executable(TerminusRuntime ${TE_RUNTIME_SOURCES})
endif()

set_target_properties(TerminusRuntime PROPERTIES FOLDER "runtime")

# -----------------------------------------------------------------------------------------------------------------------------
# LINKING
# -----------------------------------------------------------------------------------------------------------------------------

# Link Platform
target_link_libraries(TerminusRuntime TerminusEngine)