cmake_minimum_required(VERSION 3.0)
project(MicroKosm)

# Define the architecture target (e.g., arm, x86, etc.)
set(ARCH_TARGET "x86_64")

# Compiler flags for different architectures
if(ARCH_TARGET STREQUAL "AArch64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv7-a")
elseif(ARCH_TARGET STREQUAL "x86_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=x86-64")
endif()

# Generic compiler flags (common for all architectures)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain.cmake)

# Define source files directory
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Recursively find all .cpp files in the source directory
file(GLOB_RECURSE SRC_FILES 
    "${SOURCE_DIR}/generic/*.cpp"
)

# Exclude files in the 'arch' subdirectory that don't match the ARCH_TARGET
file(GLOB_RECURSE EXCLUDE_FILES "${SOURCE_DIR}/arch/*.cpp")
list(FILTER SRC_FILES EXCLUDE REGEX "${SOURCE_DIR}/arch/[^/]*\\.${ARCH_TARGET}\\.cpp$")

# Add an executable target
add_executable(myexecutable ${SRC_FILES})

# Include directories if needed
target_include_directories(myexecutable PRIVATE ${SOURCE_DIR})

# Link libraries if needed
# target_link_libraries(myexecutable PRIVATE some_library)

# Optionally, set specific linker flags
# target_link_options(myexecutable PRIVATE ...)


