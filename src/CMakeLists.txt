# Create the library project.
add_library(krypton)

add_definitions(-DKRYPTON_TEST_TEMPLATES)

target_sources(krypton
    PRIVATE
    ${KRYPTON_SOURCE_DIR}/lib.cpp
)

target_include_directories(
    krypton
    PUBLIC
    $<BUILD_INTERFACE:${KRYPTON_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# --- Dependencies ---

set(KRYPTON_DEPENDENCIES "")

find_package(OpenSSL)
if(OpenSSL_FOUND)
    message(STATUS "OpenSSL Found")
    list(APPEND KRYPTON_DEPENDENCIES OpenSSL::SSL)
    add_definitions(-DKRYPTON_USE_OPENSSL)
    target_link_libraries(krypton PUBLIC OpenSSL::SSL)
else()
    message(STATUS "OpenSSL Not Found")
endif()

option(KRYPTON_USE_RANGE_V3 "Use Range-V3 library for enhanced range operations" ON)

if(KRYPTON_USE_RANGE_V3)
    add_definitions(-DKRYPTON_USE_RANGE_V3)
    CPMAddPackage(
        NAME range-v3
        GITHUB_REPOSITORY ericniebler/range-v3
        GIT_TAG 0.12.0
        # the range-v3 CMakeLists screws with configuration options
        DOWNLOAD_ONLY True
    )
    if(range-v3_ADDED)
        add_library(range-v3 INTERFACE IMPORTED)
        target_include_directories(range-v3 INTERFACE "${range-v3_SOURCE_DIR}/include")

        target_link_libraries(krypton PUBLIC range-v3)
    endif()
endif()
