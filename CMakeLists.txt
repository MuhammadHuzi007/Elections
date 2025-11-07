cmake_minimum_required(VERSION 3.10)
project(ElectionDataAnalysis VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/ElectionData.cpp
    src/CSVReader.cpp
    src/ElectionAnalyzer.cpp
)

set(HEADERS
    include/ElectionRecord.h
    include/ElectionData.h
    include/CSVReader.h
    include/ElectionAnalyzer.h
)

# Main executable
add_executable(election_analysis
    ${SOURCES}
    src/main.cpp
)

# Test executable
add_executable(election_tests
    ${SOURCES}
    tests/test_main.cpp
)

# Compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Print configuration
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

