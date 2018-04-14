cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project("Terminus-Math")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(MATH_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

set(MATH_HEADERS "${PROJECT_SOURCE_DIR}/include/vec2.h"
                 "${PROJECT_SOURCE_DIR}/include/vec3.h"
                 "${PROJECT_SOURCE_DIR}/include/vec4.h"
                 "${PROJECT_SOURCE_DIR}/include/mat3.h"
                 "${PROJECT_SOURCE_DIR}/include/mat4.h"
                 "${PROJECT_SOURCE_DIR}/include/transform.h"
                 "${PROJECT_SOURCE_DIR}/include/quat.h"
                 "${PROJECT_SOURCE_DIR}/include/utility.h"
                 "${PROJECT_SOURCE_DIR}/include/simd_float4.h"
                 "${PROJECT_SOURCE_DIR}/include/simd_vec4.h"
                 "${PROJECT_SOURCE_DIR}/include/simd_mat4.h")

include_directories(${MATH_INCLUDE_DIR})

add_executable(unit_test test/main.cpp ${MATH_HEADERS})