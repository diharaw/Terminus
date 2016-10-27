cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project("simplefilewatcher")

file(GLOB_RECURSE SFW_HEADERS include/FileWatcher/*.h)

file(GLOB_RECURSE SFW_SOURCES source/*.cpp)

add_library(simplefilewatcher ${SFW_SOURCES} ${SFW_HEADERS})
set_target_properties(simplefilewatcher PROPERTIES FOLDER "Third Party/simplefilewatcher")
target_include_directories(simplefilewatcher PRIVATE "include")

