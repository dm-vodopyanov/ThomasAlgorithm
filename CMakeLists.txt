cmake_minimum_required(VERSION 2.8.6)
project (ThomasAlgorithm)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

set(OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin CACHE STRING "Output directory")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${EOUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY})

message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

add_subdirectory(src/MatrixGeneration)
add_subdirectory(src/Spline)
add_subdirectory(src/Version_Cilk)
add_subdirectory(src/Version_OpenMP)
add_subdirectory(src/Version_Sequential)
add_subdirectory(src/Version_TBB)
