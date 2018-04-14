cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# -----------------------------------------------------------------------------------------------------------------------------
# FLAGS
# -----------------------------------------------------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")


# -----------------------------------------------------------------------------------------------------------------------------
# FILES
# -----------------------------------------------------------------------------------------------------------------------------

# Headers
set(TE_GFX_HEADERS "${PROJECT_SOURCE_DIR}/include/directory.hpp"
                  "${PROJECT_SOURCE_DIR}/include/disk_directory.hpp"
                  "${PROJECT_SOURCE_DIR}/include/file.hpp"
                  "${PROJECT_SOURCE_DIR}/include/filesystem.hpp"
                  "${PROJECT_SOURCE_DIR}/include/io_macros.hpp"
                  "${PROJECT_SOURCE_DIR}/include/os_file.hpp"
                  "${PROJECT_SOURCE_DIR}/include/zip_directory.hpp"
                  "${PROJECT_SOURCE_DIR}/include/zip_file.hpp"
                  "${PROJECT_SOURCE_DIR}/include/serializer.hpp"
                  "${PROJECT_SOURCE_DIR}/include/binary_serializer.hpp"
                  "${PROJECT_SOURCE_DIR}/include/json_serializer.hpp"
                  "${PROJECT_SOURCE_DIR}/include/xml_serializer.hpp")

# Sources
set(TE_GFX_SOURCES "disk_directory.cpp"
                  "filesystem.cpp"
                  "os_file.cpp"
                  "zip_directory.cpp"
                  "zip_file.cpp")

# Source groups
source_group("Headers" FILES ${TE_GFX_HEADERS})
source_group("Sources" FILES ${TE_GFX_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# OPTIONS
# -----------------------------------------------------------------------------------------------------------------------------

set(GRAPHICS_API "Graphics API" CACHE STRING "This will determine the graphics API the RenderDevice will use. Possible Values: GLES2, GLES3, GL3, GL4, D3D11, D3D12, VK, MTL")

if(GRAPHICS_API STREQUAL "GL3")

      find_package(OpenGL REQUIRED)
      message("Graphics API : OpenGL 3.3" )
      add_definitions(-DTE_GFX_BACKEND_GL3)
      list(APPEND PROTOGFX_RENDER_DEVICE_SOURCE "render_device_gl3.cpp"
                                                "graphics_types_gl3.hpp"
                                                "gl_core_4_5.h"
                                                "gl_core_4_5.c")

elseif(GRAPHICS_API STREQUAL "GL4")

      find_package(OpenGL REQUIRED)
      message("Graphics API : OpenGL 4.0" )
      add_definitions(-DTE_GFX_BACKEND_GL4)
      list(APPEND PROTOGFX_RENDER_DEVICE_SOURCE "render_device_gl4.cpp" 
                                                "graphics_types_gl4.hpp"
                                                "gl_core_4_5.h"
                                                "gl_core_4_5.c")

elseif(GRAPHICS_API STREQUAL "GLES2")

      message("Graphics API : OpenGL ES 2.0" )
      add_definitions(-DTE_GFX_BACKEND_GLES2)
      list(APPEND PROTOGFX_RENDER_DEVICE_SOURCE "render_device_gles2.cpp" 
                                                "graphics_types_gles2.hpp")

elseif(GRAPHICS_API STREQUAL "GLES3")

      message("Graphics API : OpenGL ES 3.0" )
      add_definitions(-DTE_GFX_BACKEND_GLES3)
      list(APPEND PROTOGFX_RENDER_DEVICE_SOURCE "render_device_gles3.cpp" 
                                                "graphics_types_gles3.hpp")

elseif(GRAPHICS_API STREQUAL "VK")  

      message("Graphics API : Vulkan" )
      add_definitions(-DTE_GFX_BACKEND_VK)
      list(APPEND PROTOGFX_RENDER_DEVICE_SOURCE "render_device_vk.cpp" 
                                                "graphics_types_vk.hpp")

elseif(GRAPHICS_API STREQUAL "D3D11")

      message("Graphics API : Direct3D 11" )
      add_definitions(-DTE_GFX_BACKEND_D3D11)
      list(APPEND PROTOGFX_RENDER_DEVICE_SOURCE "render_device_d3d11.cpp" 
                                                "graphics_types_d3d11.hpp")

elseif(GRAPHICS_API STREQUAL "D3D12")

      message("Graphics API : Direct3D 12" )
      add_definitions(-DTE_GFX_BACKEND_D3D12)
      list(APPEND PROTOGFX_RENDER_DEVICE_SOURCE "render_device_d3d12.cpp" 
                                                "graphics_types_d3d12.hpp")

elseif(GRAPHICS_API STREQUAL "MTL")

      if(APPLE)
            find_library(APPKIT_LIBRARY AppKit)
            find_library(QUARTZCORE_LIBRARY QuartzCore)
            find_library(METAL_LIBRARY Metal)

            if(METAL_LIBRARY AND QUARTZCORE_LIBRARY AND APPKIT_LIBRARY)
                  
                  message("Graphics API : Metal" )
                  add_definitions(-DTE_GFX_BACKEND_MTL)
                  list(APPEND PROTOGFX_RENDER_DEVICE_SOURCE "render_device_mtl.cpp" 
                                                            "graphics_types_mtl.hpp")

                  file(GLOB_RECURSE MTLPP_SOURCE "${PROJECT_SOURCE_DIR}/external/mtlpp/src/*.hpp"
                                                 "${PROJECT_SOURCE_DIR}/external/mtlpp/src/*.cpp"
                                                 "${PROJECT_SOURCE_DIR}/external/mtlpp/src/*.mm"
                                                 "${PROJECT_SOURCE_DIR}/external/mtlpp/mtlpp.hpp"
                                                 "${PROJECT_SOURCE_DIR}/external/mtlpp/mtlpp.mm")
      
                  list(APPEND PROTOGFX_RENDER_DEVICE_SOURCE ${MTLPP_SOURCE})

            else()
                  message( FATAL_ERROR "Metal framework not found!" )
            endif()
      else()
            message( FATAL_ERROR "Metal API only available on macOS!" )
      endif()

else()
      message( FATAL_ERROR "Unknown Graphics API requested!" )
endif()

# -----------------------------------------------------------------------------------------------------------------------------
# DEFINITIONS
# -----------------------------------------------------------------------------------------------------------------------------

add_definitions(-DTE_PLATFORM_SDL2)

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

add_library(teGFX ${TE_GFX_HEADERS} ${TE_GFX_SOURCES})