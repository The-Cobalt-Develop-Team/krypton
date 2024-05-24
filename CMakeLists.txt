cmake_minimum_required(VERSION 3.14)

if(WIN32)
    cmake_policy(SET CMP0091 NEW)
endif()

project(Krypton VERSION 0.1.0)
message(STATUS "[Project] ${PROJECT_NAME}")

# Compile options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include/)
include_directories(include/krypton)
include_directories(thirdparty/)
include_directories(thirdparty/range-v3/include)

add_library(gmp thirdparty/minigmp/mini-gmp.c)

add_library(krypton src/GMPWrapper.cpp
                    src/MathCommon.cpp
                    src/Utilities.cpp
                    src/RSA.cpp
                    src/Hash.cpp
                    src/ASN1.cpp
                    src/Algorithm.cpp
                    )

option(KRYPTON_ENABLE_TEST "Build test" ON)

if(KRYPTON_ENABLE_TEST)
    message("Enable test")
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    enable_testing()
    include(GoogleTest)

    add_executable(unittests tests/MathCommonUnitTest.cpp
                            tests/GMPWrapperUnitTest.cpp
                            tests/UtilitiesUnitTest.cpp
                            tests/AESUnitTest.cpp
                            tests/RSAUnitTest.cpp
                            tests/HashUnitTest.cpp
                            tests/FunctorTest.cpp
                            tests/AlgorithmUnitTest.cpp
                            tests/ASN1UnitTest.cpp
                            tests/BlockModeUnitTest.cpp
                            tests/CPSBaseUnitTest.cpp
                            )
endif()

if(WIN32)
    if(KRYPTON_ENABLE_TEST)
        set_property(TARGET unittests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
    set_property(TARGET krypton PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_property(TARGET gmp PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(KRYPTON_ENABLE_TEST)
    target_link_libraries(unittests gmp krypton GTest::gtest_main)
    gtest_discover_tests(unittests)
endif()
