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

# Sources
set(TE_IO_TEST_SOURCES "test_io.cpp")

# Source groups
source_group("Sources" FILES ${TE_IO_TEST_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

if(APPLE)
    add_executable(test_io MACOSX_BUNDLE ${TE_IO_TEST_SOURCES})
elseif(WIN32)
    add_executable(test_io WIN32 ${TE_IO_TEST_SOURCES})
else()
    add_executable(test_io ${TE_IO_TEST_SOURCES})
endif()

set_target_properties(test_io PROPERTIES FOLDER "tests")

# -----------------------------------------------------------------------------------------------------------------------------
# LINKING
# -----------------------------------------------------------------------------------------------------------------------------

# Link Platform
target_link_libraries(test_io teIO)
target_link_libraries(test_io teMemory)
target_link_libraries(test_io teEvent)
target_link_libraries(test_io tePlatform)
target_link_libraries(test_io debug "${TE_LIBZIP_DEBUG_LIB}" optimized "${TE_LIBZIP_RELEASE_LIB}")
target_link_libraries(test_io debug "${TE_ZLIB_DEBUG_LIB}" optimized "${TE_ZLIB_RELEASE_LIB}")
target_link_libraries(test_io debug "${TE_SDL2_DEBUG_LIB}" optimized "${TE_SDL2_RELEASE_LIB}")
target_link_libraries(test_io debug "${TE_SDL2_MAIN_DEBUG_LIB}" optimized "${TE_SDL2_MAIN_RELEASE_LIB}")

if(APPLE)
    # Link Apple Frameworks
    target_link_libraries(test_io "-framework CoreVideo"
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

    set(MACOSX_BUNDLE_BUNDLE_NAME "Terminus Test - IO")
    set(MAC_HIGH_RESOLUTION_CAPABLE "true")
endif() 