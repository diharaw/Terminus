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
target_link_libraries(test_io teConcurrency)
target_link_libraries(test_io "${TE_LIBZIP_LIBS}")
target_link_libraries(test_io "${TE_ZLIB_LIBS}")
target_link_libraries(test_io "${TE_SDL2_LIBS}")

if(WIN32)
    target_link_libraries(test_io "user32.lib"
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