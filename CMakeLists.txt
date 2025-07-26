cmake_minimum_required(VERSION 3.20)
project(StudentActivityAssignment VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for modern C++
if(MSVC)
    add_compile_options(/W4 /std:c++20)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -std=c++20)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Create executable
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy input files to build directory
configure_file(${CMAKE_SOURCE_DIR}/students.txt ${CMAKE_BINARY_DIR}/students.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/activities.txt ${CMAKE_BINARY_DIR}/activities.txt COPYONLY)

# Add custom target for running the application
add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${PROJECT_NAME}
    COMMENT "Running Student Activity Assignment System"
)
