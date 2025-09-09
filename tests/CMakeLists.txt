# Enable testing for this directory and subdirectories.
enable_testing()

# Find the GTest package.
# This defines the GTest::gtest and GTest::gtest_main imported targets.
CPMAddPackage(
    NAME gtest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG v1.17.0
    VERSION 1.17.0
    OPTIONS
    "INSTALL_GTEST OFF"
    "gtest_force_shared_crt ON"
)

# Include the GoogleTest module which provides gtest_discover_tests.
include(GoogleTest)

# Create the test executable.
# Replace test_main.cpp with your actual test source files.
add_executable(
    krypton_tests
    test_main.cpp
    common/secure_vector_test.cpp
)

# Link the test executable against GTest and the project's main target.
# Replace ${PROJECT_NAME} with the name of your library or executable target.
target_link_libraries(
    krypton_tests
    PRIVATE
    GTest::gtest
    krypton
)

# Automatically discover and add tests to CTest.
gtest_discover_tests(krypton_tests)
