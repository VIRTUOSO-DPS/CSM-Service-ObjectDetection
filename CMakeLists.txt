cmake_minimum_required(VERSION 3.16)

# Set the CUDA architecture to 5.3, supported by Jetson Nano
set(CMAKE_CUDA_ARCHITECTURES 53)

# Generate compile commands JSON
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use C++17
set(CMAKE_CXX_STANDARD 17)

project(CSM-Service-Edge-ObjectDetection
    LANGUAGES CXX
)

option(ENABLE_CUDA "Enable CUDA support" ON)

if(ENABLE_CUDA)
    enable_language(CUDA)
endif()

# We require OpenCV to be available
find_package(OpenCV REQUIRED)

# Global variables
set(PROJECT_ROOT     "${CMAKE_CURRENT_SOURCE_DIR}")
set(SOURCE_ROOT      "${PROJECT_ROOT}/src")
set(THIRD_PARTY_ROOT "${PROJECT_ROOT}/3rd")

set(SERVICE_TARGET   "csm-service-objectdetection")

# Create main target
add_executable(${SERVICE_TARGET})

# Include external dependencies
add_subdirectory(3rd)

# Include source
add_subdirectory(src)

# Link libraries
target_link_libraries(${SERVICE_TARGET}
    PRIVATE
        # Crow
        Crow

        # OpenCV
        ${OpenCV_LIBS}
)

# Include directories
target_include_directories(${SERVICE_TARGET}
    PRIVATE
        # OpenCV
        ${OpenCV_INCLUDE_DIRS}

        stdc++fs
)

# Copy resources directory to the binary directory
add_custom_command(TARGET ${SERVICE_TARGET} 
    POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_ROOT}/resources" "${CMAKE_BINARY_DIR}/resources"
)
add_custom_target(CopyResources ALL DEPENDS ${SERVICE_TARGET} )