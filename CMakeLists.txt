cmake_minimum_required(VERSION 3.16)

# Set the CUDA architecture to 5.3, supported by Jetson Nano
set(CMAKE_CUDA_ARCHITECTURES 53)

# Generate compile commands JSON
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use C++17
set(CMAKE_CXX_STANDARD 17)

project(CSM-Service-Edge-ObjectDetection
    LANGUAGES CXX CUDA
)

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
)

# Copy resources directory to the binary directory
add_custom_command(TARGET ${SERVICE_TARGET} 
    POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_ROOT}/resources" "${CMAKE_BINARY_DIR}/resources"
)
add_custom_target(CopyResources ALL DEPENDS ${SERVICE_TARGET} )

# Installation paths
set(INSTALL_PREFIX "/opt/csm-service-objectdetection")

# Install executable
install(
    TARGETS 
        ${SERVICE_TARGET} 
    DESTINATION 
        ${INSTALL_PREFIX}
)

# Install resources directory
install(
    DIRECTORY 
        "${PROJECT_ROOT}/resources" 
    DESTINATION 
        ${INSTALL_PREFIX}
)

# Install service
install(
    FILES 
        "${PROJECT_ROOT}/csm-service-objectdetection.service"
    DESTINATION 
        /etc/systemd/system
)