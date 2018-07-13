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
# FILES - CONCURRENCY
# -----------------------------------------------------------------------------------------------------------------------------

# Headers
set(TE_CONCURRENCY_HEADERS "${CMAKE_SOURCE_DIR}/engine/include/concurrency/atomic.hpp"
                           "${CMAKE_SOURCE_DIR}/engine/include/concurrency/lock.hpp"
                           "${CMAKE_SOURCE_DIR}/engine/include/concurrency/mutex.hpp"
                           "${CMAKE_SOURCE_DIR}/engine/include/concurrency/scoped_lock.hpp"
                           "${CMAKE_SOURCE_DIR}/engine/include/concurrency/semaphore.hpp"
                           "${CMAKE_SOURCE_DIR}/engine/include/concurrency/thread.hpp"
                           "${CMAKE_SOURCE_DIR}/engine/include/concurrency/thread_common.hpp")
           
# Sources
set(TE_CONCURRENCY_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/concurrency/lock.cpp"
                           "${CMAKE_SOURCE_DIR}/engine/src/concurrency/mutex.cpp"
                           "${CMAKE_SOURCE_DIR}/engine/src/concurrency/scoped_lock.cpp"
                           "${CMAKE_SOURCE_DIR}/engine/src/concurrency/semaphore.cpp"
                           "${CMAKE_SOURCE_DIR}/engine/src/concurrency/thread.cpp")

# -----------------------------------------------------------------------------------------------------------------------------
# FILES - CORE
# -----------------------------------------------------------------------------------------------------------------------------

# Headers
set(TE_CORE_HEADERS "${CMAKE_SOURCE_DIR}/engine/include/core/application.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/core/engine_core.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/core/platform_macros.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/core/terminus_macros.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/core/terminus_types.hpp")
           
# Sources
set(TE_CORE_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/core/application.cpp"
                    "${CMAKE_SOURCE_DIR}/engine/src/core/engine_core.cpp")

# -----------------------------------------------------------------------------------------------------------------------------
# FILES - EVENT
# -----------------------------------------------------------------------------------------------------------------------------

# Headers
set(TE_EVENT_HEADERS "${CMAKE_SOURCE_DIR}/engine/include/event/event.hpp"
                     "${CMAKE_SOURCE_DIR}/engine/include/event/event_macros.hpp"
                     "${CMAKE_SOURCE_DIR}/engine/include/event/event_manager.hpp")
           
# Sources
set(TE_EVENT_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/event/event_manager.cpp")

# -----------------------------------------------------------------------------------------------------------------------------
# FILES - IO
# -----------------------------------------------------------------------------------------------------------------------------

# Headers
set(TE_IO_HEADERS "${CMAKE_SOURCE_DIR}/engine/include/io/file.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/filesystem.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/input_device.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/input_manager.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/input_map.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/io_macros.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/stream.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/file_stream.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/memory_stream.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/serializer.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/json_serializer.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/binary_serializer.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/reflection.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/archive.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/os_file.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/zip_file.hpp"
                  "${CMAKE_SOURCE_DIR}/engine/include/io/type_descriptor.hpp")

# Sources
set(TE_IO_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/io/filesystem.cpp"
                  "${CMAKE_SOURCE_DIR}/engine/src/io/input_manager.cpp"
                  "${CMAKE_SOURCE_DIR}/engine/src/io/input_map.cpp"
                  "${CMAKE_SOURCE_DIR}/engine/src/io/os_file.cpp"
                  "${CMAKE_SOURCE_DIR}/engine/src/io/archive.cpp"
                  "${CMAKE_SOURCE_DIR}/engine/src/io/zip_file.cpp"
                  "${CMAKE_SOURCE_DIR}/engine/src/io/binary_serializer.cpp"
                  "${CMAKE_SOURCE_DIR}/engine/src/io/json_serializer.cpp"
                  "${CMAKE_SOURCE_DIR}/engine/src/io/file_stream.cpp"
                  "${CMAKE_SOURCE_DIR}/engine/src/io/memory_stream.cpp"
                  "${CMAKE_SOURCE_DIR}/engine/src/io/type_descriptor.cpp")

# -----------------------------------------------------------------------------------------------------------------------------
# FILES - MATH
# -----------------------------------------------------------------------------------------------------------------------------

# Headers
set(TE_MATH_HEADERS "${CMAKE_SOURCE_DIR}/engine/include/math/mat3.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/mat4.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/quat.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/simd_float4.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/simd_float8.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/simd_mat4x4.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/simd_mat4x8.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/simd_vec4x4.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/simd_vec4x8.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/transform.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/utility.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/vec2.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/vec3.hpp"
                    "${CMAKE_SOURCE_DIR}/engine/include/math/vec4.hpp")

# -----------------------------------------------------------------------------------------------------------------------------
# FILES - MEMORY
# -----------------------------------------------------------------------------------------------------------------------------

# Headers
set(TE_MEMORY_HEADERS "${CMAKE_SOURCE_DIR}/engine/include/memory/allocator.hpp"
                      "${CMAKE_SOURCE_DIR}/engine/include/memory/buffer_linear_allocator.hpp"
                      "${CMAKE_SOURCE_DIR}/engine/include/memory/buffer_pool_allocator.hpp"
                      "${CMAKE_SOURCE_DIR}/engine/include/memory/heap_allocator.hpp"
                      "${CMAKE_SOURCE_DIR}/engine/include/memory/linear_allocator.hpp"
                      "${CMAKE_SOURCE_DIR}/engine/include/memory/memory_common.hpp"
                      "${CMAKE_SOURCE_DIR}/engine/include/memory/memory_macros.hpp"
                      "${CMAKE_SOURCE_DIR}/engine/include/memory/pool_allocator.hpp")

# Sources
set(TE_MEMORY_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/memory/heap_allocator.cpp"
                      "${CMAKE_SOURCE_DIR}/engine/src/memory/linear_allocator.cpp"
                      "${CMAKE_SOURCE_DIR}/engine/src/memory/pool_allocator.cpp")

# -----------------------------------------------------------------------------------------------------------------------------
# FILES - PLATFORM
# -----------------------------------------------------------------------------------------------------------------------------

# Headers
set(TE_PLATFORM_HEADERS "${CMAKE_SOURCE_DIR}/engine/include/platform/allocator.hpp"
                        "${CMAKE_SOURCE_DIR}/engine/include/platform/buffer_linear_allocator.hpp"
                        "${CMAKE_SOURCE_DIR}/engine/include/platform/buffer_pool_allocator.hpp"
                        "${CMAKE_SOURCE_DIR}/engine/include/platform/heap_allocator.hpp"
                        "${CMAKE_SOURCE_DIR}/engine/include/platform/linear_allocator.hpp"
                        "${CMAKE_SOURCE_DIR}/engine/include/platform/memory_common.hpp"
                        "${CMAKE_SOURCE_DIR}/engine/include/platform/memory_macros.hpp"
                        "${CMAKE_SOURCE_DIR}/engine/include/platform/pool_allocator.hpp")

# Sources
set(TE_PLATFORM_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/platform/window.cpp")

# -----------------------------------------------------------------------------------------------------------------------------
# FILES - STL
# -----------------------------------------------------------------------------------------------------------------------------

# Headers
set(TE_STL_HEADERS "${CMAKE_SOURCE_DIR}/engine/include/stl/array.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/stl/deque.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/stl/murmur_hash.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/stl/packed_array.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/stl/sparse_array.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/stl/static_hash_map.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/stl/string_buffer.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/stl/string_hash.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/stl/vector.hpp")

# -----------------------------------------------------------------------------------------------------------------------------
# FILES - GFX
# -----------------------------------------------------------------------------------------------------------------------------

# Headers
set(TE_GFX_HEADERS "${CMAKE_SOURCE_DIR}/engine/include/gfx/gfx_descs.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/gfx_device.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/gfx_enums.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/gfx_types.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/vulkan/gfx_device_vk.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/vulkan/gfx_types_vk.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/vulkan/gfx_initializers_vk.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/vulkan/vk_mem_alloc.h"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/opengl/gfx_device_gl.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/opengl/gfx_types_gl.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/opengl/glad.h"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/opengl/khrplatform.h"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/direct3d11/gfx_device_d3d11.hpp"
                   "${CMAKE_SOURCE_DIR}/engine/include/gfx/direct3d11/gfx_types_d3d11.hpp")

# Sources
set(TE_GFX_SOURCES "${CMAKE_SOURCE_DIR}/engine/src/gfx/vulkan/gfx_device_vk.cpp"
                   "${CMAKE_SOURCE_DIR}/engine/src/gfx/opengl/gfx_device_gl.cpp"
                   "${CMAKE_SOURCE_DIR}/engine/src/gfx/opengl/glad.c"
                   "${CMAKE_SOURCE_DIR}/engine/src/gfx/direct3d11/gfx_device_d3d11.cpp")

# Terminus Headers
set(TE_HEADERS ${TE_CORE_HEADERS} 
               ${TE_CONCURRENCY_HEADERS}
               ${TE_EVENT_HEADERS} 
               ${TE_IO_HEADERS}
               ${TE_MATH_HEADERS}
               ${TE_MEMORY_HEADERS}
               ${TE_STL_HEADERS}
               ${TE_GFX_HEADERS})

# Terminus Source
set(TE_SOURCES ${TE_CORE_SOURCES} 
               ${TE_CONCURRENCY_SOURCES} 
               ${TE_EVENT_SOURCES} 
               ${TE_IO_SOURCES}
               ${TE_MEMORY_SOURCES}
               ${TE_GFX_SOURCES})

# Source groups
source_group("include" FILES ${TE_HEADERS})
source_group("src" FILES  ${TE_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

add_library(TerminusEngine ${TE_HEADERS} ${TE_SOURCES})

set_target_properties(TerminusEngine PROPERTIES FOLDER "engine")

target_link_libraries(TerminusEngine "${TE_LIBZIP_LIBS}")
target_link_libraries(TerminusEngine "${TE_ZLIB_LIBS}")
target_link_libraries(TerminusEngine "${TE_SDL2_LIBS}")
target_link_libraries(TerminusEngine "${VULKAN_LIBRARY}")
target_link_libraries(TerminusEngine "${OPENGL_LIBRARIES}")

if(WIN32)
    target_link_libraries(TerminusEngine "user32.lib"
                                         "gdi32.lib"
                                         "winmm.lib"
                                         "imm32.lib"
                                         "ole32.lib"
                                         "oleaut32.lib"
                                         "version.lib"
                                         "uuid.lib"
                                         "dinput8.lib"
                                         "kernel32.lib"
                                         "winspool.lib"
                                         "shell32.lib"
                                         "comdlg32.lib"
                                         "advapi32.lib")
elseif(APPLE)
    # Link Apple Frameworks
    target_link_libraries(TerminusEngine "-framework CoreVideo"
                                         "-framework Cocoa"
                                         "-framework CoreAudio"
                                         "-framework AudioUnit"
                                         "-framework AudioToolbox"
                                         "-framework Carbon"
                                         "-framework ForceFeedback"
                                         "-framework IOKit"
                                         "-framework ApplicationServices"
                                         "-liconv"
                                         "-framework QuartzCore")
endif() 