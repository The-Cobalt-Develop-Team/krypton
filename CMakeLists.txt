cmake_minimum_required(VERSION 3.14)

project(Krypton VERSION 0.1.0)
message(STATUS "[Project] ${PROJECT_NAME}")

# Compile options
# set(CMAKE_C_COMPILER "clang")
# set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "-fno-asm -std=c++17 -Wall")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -Og -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include/)
include_directories(thirdparty/)

set(INSTALL_GTEST OFF)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/gtest)

include_directories(thirdparty/gtest/googletest/include)

add_library(gmp thirdparty/minigmp/mini-gmp.c)

# add_executable(test1 src/Tests/TestBigInteger1.cpp)
# add_executable(test2 src/Tests/TestMathCommon1.cpp)
add_executable(unittests src/Tests/MathCommonUnitTest.cpp)

# target_link_libraries(test1 gmp)
# target_link_libraries(test2 gmp)
target_link_libraries(unittests gmp GTest::gtest_main)
