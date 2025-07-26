cmake_minimum_required(VERSION 3.20)
project(StudentActivityAssignment VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for modern C++
if(MSVC)
    add_compile_options(/W4 /std:c++23)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -std=c++23)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Create executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/application/services/ActivityAssignmentService.cpp
    src/application/strategies/IRandomSelectionStrategy.cpp
    src/domain/entities/Activity.cpp
    src/domain/entities/Student.cpp
    src/infrastructure/repositories/FileActivityRepository.cpp
    src/infrastructure/repositories/FileStudentRepository.cpp
    src/presentation/controllers/ActivityAssignmentController.cpp
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy data files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/data/ DESTINATION ${CMAKE_BINARY_DIR}/data/)

# Add custom target for running the application
add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${PROJECT_NAME}
    COMMENT "Running Student Activity Assignment System"
)
