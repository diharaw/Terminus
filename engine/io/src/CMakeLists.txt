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
set(TE_IO_PUBLIC_HEADERS "${PROJECT_SOURCE_DIR}/include/file.hpp"
                         "${PROJECT_SOURCE_DIR}/include/filesystem.hpp"
                         "${PROJECT_SOURCE_DIR}/include/input_device.hpp"
                         "${PROJECT_SOURCE_DIR}/include/input_manager.hpp"
                         "${PROJECT_SOURCE_DIR}/include/input_map.hpp"
                         "${PROJECT_SOURCE_DIR}/include/io_macros.hpp"
                         "${PROJECT_SOURCE_DIR}/include/stream.hpp"
                         "${PROJECT_SOURCE_DIR}/include/file_stream.hpp"
                         "${PROJECT_SOURCE_DIR}/include/memory_stream.hpp"
                         "${PROJECT_SOURCE_DIR}/include/serializer.hpp"
                         "${PROJECT_SOURCE_DIR}/include/json_serializer.hpp"
                         "${PROJECT_SOURCE_DIR}/include/xml_serializer.hpp"
                         "${PROJECT_SOURCE_DIR}/include/binary_serializer.hpp"
                         "${PROJECT_SOURCE_DIR}/include/reflection.hpp"
                         "${PROJECT_SOURCE_DIR}/include/type_descriptor.hpp")

# Private Headers
set(TE_IO_PRIVATE_HEADERS "${PROJECT_SOURCE_DIR}/src/filesystem_impl.hpp"
                          "${PROJECT_SOURCE_DIR}/src/input_manager_impl.hpp"
                          "${PROJECT_SOURCE_DIR}/src/input_map_impl.hpp"
                          "${PROJECT_SOURCE_DIR}/src/archive.hpp"
                          "${PROJECT_SOURCE_DIR}/src/os_file.hpp"
                          "${PROJECT_SOURCE_DIR}/src/zip_file.hpp")

# Sources
set(TE_IO_SOURCES "filesystem_impl.cpp"
                  "input_manager_impl.cpp"
                  "input_map_impl.cpp"
                  "os_file.cpp"
                  "archive.cpp"
                  "zip_file.cpp"
                  "binary_serializer.cpp"
                  "json_serializer.cpp"
                  "xml_serializer.cpp"
                  "file_stream.cpp"
                  "memory_stream.cpp"
                  "type_descriptor.cpp")

# Source groups
source_group("public" FILES ${TE_IO_PUBLIC_HEADERS})
source_group("private" FILES ${TE_IO_PRIVATE_HEADERS})
source_group("sources" FILES ${TE_IO_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

add_library(teIO ${TE_IO_PUBLIC_HEADERS} ${TE_IO_PRIVATE_HEADERS} ${TE_IO_SOURCES})
set_target_properties(teIO PROPERTIES FOLDER "engine")