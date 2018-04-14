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
set(TE_PLATFORM_TEST_SOURCES "test_platform.cpp")

# Source groups
source_group("Sources" FILES ${TE_PLATFORM_TEST_SOURCES})

# -----------------------------------------------------------------------------------------------------------------------------
# TARGETS
# -----------------------------------------------------------------------------------------------------------------------------

if(APPLE)
    add_executable(test_platform MACOSX_BUNDLE ${TE_PLATFORM_TEST_SOURCES})
elseif(WIN32)
    add_executable(test_platform WIN32 ${TE_PLATFORM_TEST_SOURCES})
else()
    add_executable(test_platform ${TE_PLATFORM_TEST_SOURCES})
endif()

set_target_properties(test_platform PROPERTIES FOLDER "tests")

# -----------------------------------------------------------------------------------------------------------------------------
# LINKING
# -----------------------------------------------------------------------------------------------------------------------------

# Link Platform
target_link_libraries(test_platform tePlatform)
target_link_libraries(test_platform "${TE_SDL2_LIBS}")

if(WIN32)
    target_link_libraries(test_platform "user32.lib"
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
    target_link_libraries(test_platform "-framework CoreVideo"
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

    set(MACOSX_BUNDLE_BUNDLE_NAME "Terminus Test - Platform")
    set(MAC_HIGH_RESOLUTION_CAPABLE "true")
endif() 