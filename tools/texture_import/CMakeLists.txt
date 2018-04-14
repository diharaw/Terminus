cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project("TextureImport")

add_subdirectory(external/nvidia-texture-tools)
add_subdirectory(external/cmft)

# Headers
set(TI_HEADERS "${PROJECT_SOURCE_DIR}/src/stb_image.h"
               "${PROJECT_SOURCE_DIR}/src/stb_image_write.h"
               "${PROJECT_SOURCE_DIR}/src/stb_image_resize.h")

# Sources
set(TI_SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")

# Source groups
source_group("Headers" FILES ${TI_HEADERS})
source_group("Sources" FILES ${TI_SOURCES})

# Include paths
set(TI_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/external/nvidia-texture-tools/src"
                    "${PROJECT_SOURCE_DIR}/external/cmft/include")

include_directories("${TI_INCLUDE_DIRS}")

if(APPLE)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
endif()

add_executable(TextureImport ${TI_HEADERS} ${TI_SOURCES})
target_link_libraries(TextureImport nvtt)
target_link_libraries(TextureImport cmft)