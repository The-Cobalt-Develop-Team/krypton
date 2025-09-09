cmake_minimum_required(VERSION 3.28)
project(krypton VERSION 1.0.0 LANGUAGES CXX)

# Set the C++ standard. This is a public requirement for a header-only library.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define some CMake variables for convenience.
set(KRYPTON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(KRYPTON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(KRYPTON_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(KRYPTON_EXAMPLE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)

# --- CPM.cmake ---
include(cmake/CPM.cmake)

# --- Library Source ---
add_subdirectory(${KRYPTON_SOURCE_DIR})

# --- Optional: Testing ---
# Enable testing with CTest.
option(BUILD_TESTING "Build the tests" ON)
if(BUILD_TESTING)
    message(STATUS "Enable tests")
    add_subdirectory(${KRYPTON_TEST_DIR})
endif()

# --- Optional: Example ---
option(BUILD_EXAMPLES "Build the examples" OFF)
if(BUILD_EXAMPLES)
    message(STATUS "Build examples")
    add_subdirectory(${KRYPTON_EXAMPLE_DIR})
endif()
