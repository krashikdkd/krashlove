cmake_minimum_required(VERSION 3.15)
project(KrashLove VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform check
if(NOT WIN32)
    message(FATAL_ERROR "This project is designed for Windows only!")
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/optimizer.cpp
)

# Header files
set(HEADERS
    include/optimizer.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link Windows libraries
target_link_libraries(${PROJECT_NAME}
    psapi
    iphlpapi
    advapi32
    shell32
)

# Set warning levels
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Print build information
message(STATUS "Building KrashLove Windows Optimizer")
message(STATUS "C++ Standard: C++17")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
