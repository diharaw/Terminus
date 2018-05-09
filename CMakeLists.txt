cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# -----------------------------------------------------------------------------------------------------------------------------
# PROJECT
# -----------------------------------------------------------------------------------------------------------------------------

project("Terminus")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# -----------------------------------------------------------------------------------------------------------------------------
# DEPENDANCIES
# -----------------------------------------------------------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Setup platform specific library options
if(WIN32)
    set(TE_LIB_PREFFIX "")
    set(TE_STATIC_LIB_SUFFIX ".lib")
    set(TE_SHARED_LIB_SUFFIX ".dll")
elseif(APPLE)
    set(TE_LIB_PREFFIX "lib")
    set(TE_STATIC_LIB_SUFFIX ".a")
    set(TE_SHARED_LIB_SUFFIX ".dylib")
else()
    set(TE_LIB_PREFFIX "lib")
    set(TE_STATIC_LIB_SUFFIX ".o")
    set(TE_SHARED_LIB_SUFFIX ".so")
endif()

# Setup library paths
set(TE_LIB_PATH "${PROJECT_SOURCE_DIR}/libs")

# SDL2
set(TE_SDL2_LIBS "${TE_LIB_PATH}/${TE_LIB_PREFFIX}SDL2${TE_STATIC_LIB_SUFFIX}" 
                 "${TE_LIB_PATH}/${TE_LIB_PREFFIX}SDL2main${TE_STATIC_LIB_SUFFIX}")

# LibZip
set(TE_LIBZIP_LIBS "${TE_LIB_PATH}/${TE_LIB_PREFFIX}zipstatic${TE_STATIC_LIB_SUFFIX}")

# Zlib
if(WIN32)
    set(TE_ZLIB_LIBS "${TE_LIB_PATH}/${TE_LIB_PREFFIX}zlibstatic${TE_STATIC_LIB_SUFFIX}")
else()
    set(TE_ZLIB_LIBS "${TE_LIB_PATH}/${TE_LIB_PREFFIX}z${TE_STATIC_LIB_SUFFIX}")
endif()

# Recast
set(TE_RECAST_LIBS "${TE_LIB_PATH}/${TE_LIB_PREFFIX}Recast${TE_STATIC_LIB_SUFFIX}")

# Detour
set(TE_DETOUR_LIBS "${TE_LIB_PATH}/${TE_LIB_PREFFIX}Detour${TE_STATIC_LIB_SUFFIX}")

# Detour Crowd
set(TE_DETOUR_CROWD_LIBS "${TE_LIB_PATH}/${TE_LIB_PREFFIX}DetourCrowd${TE_STATIC_LIB_SUFFIX}")

# Detour Tile Cache
set(TE_DETOUR_TILE_CACHE_LIBS "${TE_LIB_PATH}/${TE_LIB_PREFFIX}DetourTileCache${TE_STATIC_LIB_SUFFIX}")

# Assimp
set(TE_ASSIMP_LIBS "${TE_LIB_PATH}/${TE_LIB_PREFFIX}assimp${TE_STATIC_LIB_SUFFIX}")

# -----------------------------------------------------------------------------------------------------------------------------
# INCLUDES
# -----------------------------------------------------------------------------------------------------------------------------

find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)

# Setup engine include paths
set(TE_ENGINE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/engine/include")

# Setup external include paths
set(TE_SDL2_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/SDL/include")
set(TE_LIBZIP_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/libzip")
set(TE_ASSIMP_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/assimp/include")
set(TE_JSON_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/json/single_include/nlohmann")

# Set includes
include_directories("${TE_ENGINE_INCLUDE_PATH}"
                    "${TE_SDL2_INCLUDE_PATH}"
                    "${TE_LIBZIP_INCLUDE_PATH}"
                    "${TE_ASSIMP_INCLUDE_PATH}"
                    "${TE_JSON_INCLUDE_PATH}"
                    "${VULKAN_INCLUDE_DIR}")

# -----------------------------------------------------------------------------------------------------------------------------
# SUB-DIRECTORIES
# -----------------------------------------------------------------------------------------------------------------------------                    

add_subdirectory("engine/src")
add_subdirectory("runtime")