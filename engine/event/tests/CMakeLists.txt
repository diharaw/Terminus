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
set(TE_EVENT_TEST_SOURCES "test_event.cpp")

# Source groups
source_group("Sources" FILES ${TE_EVENT_TEST_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

add_executable(test_event ${TE_EVENT_TEST_SOURCES})
set_target_properties(test_event PROPERTIES FOLDER "tests")

# -----------------------------------------------------------------------------------------------------------------------------
# LINKING
# -----------------------------------------------------------------------------------------------------------------------------

# Link Platform
target_link_libraries(test_event teEvent)
target_link_libraries(test_event tePlatform)
target_link_libraries(test_event debug "${TE_SDL2_DEBUG_LIB}" optimized "${TE_SDL2_RELEASE_LIB}")
target_link_libraries(test_event debug "${TE_SDL2_MAIN_DEBUG_LIB}" optimized "${TE_SDL2_MAIN_RELEASE_LIB}")

if(APPLE)
    # Link Apple Frameworks
    target_link_libraries(test_event "-framework CoreVideo"
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

    set(MACOSX_BUNDLE_BUNDLE_NAME "Terminus Test - Event")
    set(MAC_HIGH_RESOLUTION_CAPABLE "true")
endif() 