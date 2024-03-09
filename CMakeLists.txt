cmake_minimum_required(VERSION 3.14)

if(WIN32)
    cmake_policy(SET CMP0091 NEW)
endif()

project(Krypton VERSION 0.1.0)
message(STATUS "[Project] ${PROJECT_NAME}")

# Compile options
# set(CMAKE_C_COMPILER "clang")
# set(CMAKE_CXX_COMPILER "clang++")
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "-fno-asm -std=c++17 -Wall")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -Og -fsanitize=address")
# set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include/)
include_directories(thirdparty/)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_library(gmp thirdparty/minigmp/mini-gmp.c)

# TODO: remove ASN1 external dependence
# add_library(asn1 thirdparty/asn1/ber/common.cpp
#                  thirdparty/asn1/ber/decoder.cpp
#                  thirdparty/asn1/ber/tag.cpp)

add_library(krypton src/GMPWrapper.cpp
                    src/MathCommon.cpp
                    src/Utilities.cpp
                    src/RSA.cpp
                    src/Hash.cpp
                    # src/ASN1.cpp
                    )

enable_testing()
include(GoogleTest)

add_executable(unittests tests/MathCommonUnitTest.cpp
                         tests/GMPWrapperUnitTest.cpp
                         tests/UtilitiesUnitTest.cpp
                         tests/AESUnitTest.cpp
                         tests/HashUnitTest.cpp
                         tests/FunctorTest.cpp
                         tests/AlgorithmUnitTest.cpp
                        #  tests/ASN1UnitTest.cpp
                         )

if(WIN32)
    set_property(TARGET unittests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_property(TARGET krypton PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_property(TARGET gmp PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    # set_property(TARGET asn1 PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

target_link_libraries(unittests gmp krypton GTest::gtest_main)

gtest_discover_tests(unittests)
