# Create the library project.
add_library(krypton)

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


