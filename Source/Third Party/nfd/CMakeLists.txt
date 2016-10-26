# Project definition
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(nfd)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(LINUX FALSE)
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   set(LINUX TRUE)
endif()

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCES "${INC_DIR}/nfd.h")
list(APPEND SOURCES "${SRC_DIR}/nfd_common.h")
list(APPEND SOURCES "${SRC_DIR}/nfd_common.c")
list(APPEND SOURCES "${SRC_DIR}/common.h")

if (APPLE)
   list(APPEND SOURCES
      "${SRC_DIR}/nfd_cocoa.m"
   )
elseif (WIN32)
   list(APPEND SOURCES
      "${SRC_DIR}/nfd_win.cpp"
   )
elseif (LINUX)
   list(APPEND SOURCES
      "${SRC_DIR}/nfd_gtk.c"
   )
endif ()

# Library definition and features
add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${INC_DIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Third Party")

# Platform-specific dependencies
if (APPLE)
   find_library(COCOA_LIBRARY Cocoa)
   target_link_libraries(${PROJECT_NAME} PUBLIC ${COCOA_LIBRARY})
elseif (LINUX)
   find_package(PkgConfig REQUIRED)
   pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
   target_link_libraries(${PROJECT_NAME} PUBLIC ${GTK3_LIBRARIES})
   target_include_directories(${PROJECT_NAME} PRIVATE ${GTK3_INCLUDE_DIRS})
endif ()
