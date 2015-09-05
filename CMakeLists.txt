cmake_minimum_required(VERSION 2.8.4)
project(SmartPointers)

# useful variables
set(SourcePath ${PROJECT_SOURCE_DIR}/src)
set(TestPath ${PROJECT_SOURCE_DIR}/test)
set(LibPath ${PROJECT_SOURCE_DIR}/lib)

# include directories
include_directories(${SourcePath})
include_directories(${TestPath})

# CMAKE flags
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)

# C++ compilation flags
# no flags :)

include(CMakeSrc.cmake)

add_executable(Test ${SOURCE_FILES})
target_link_libraries(Test debug ${LibPath}/gtestd${CMAKE_FIND_LIBRARY_SUFFIXES})
target_link_libraries(Test optimized ${LibPath}/gtest${CMAKE_FIND_LIBRARY_SUFFIXES})

# create output directory
add_custom_command(TARGET Test PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
