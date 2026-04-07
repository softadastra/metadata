cmake_minimum_required(VERSION 3.20)

project(softadastra_metadata
    VERSION 0.1.0
    DESCRIPTION "Softadastra Metadata - Node identity and runtime metadata layer"
    LANGUAGES CXX
)

include(GNUInstallDirs)

# ========================
# Options
# ========================
option(SOFTADASTRA_METADATA_BUILD_TESTS "Build metadata tests" OFF)
option(SOFTADASTRA_METADATA_BUILD_EXAMPLES "Build metadata examples" OFF)

# ========================
# Global settings
# ========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ========================
# Dependencies
# ========================
if(NOT SOFTADASTRA_UMBRELLA_BUILD)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.vix/vix_deps.cmake")
        include("${CMAKE_CURRENT_SOURCE_DIR}/.vix/vix_deps.cmake")
    endif()
endif()

if(NOT TARGET softadastra::discovery AND NOT TARGET softadastra_discovery)
    find_package(softadastra_discovery CONFIG QUIET)
    find_package(discovery CONFIG QUIET)
endif()

set(SOFTADASTRA_METADATA_DISCOVERY_TARGET "")
if(TARGET softadastra::discovery)
    set(SOFTADASTRA_METADATA_DISCOVERY_TARGET softadastra::discovery)
elseif(TARGET softadastra_discovery)
    set(SOFTADASTRA_METADATA_DISCOVERY_TARGET softadastra_discovery)
endif()

if(SOFTADASTRA_METADATA_DISCOVERY_TARGET STREQUAL "")
    message(FATAL_ERROR
        "softadastra discovery target not found.\n"
        "Install dependencies with Vix:\n"
        "  vix add @softadastra/discovery\n"
        "  vix install"
    )
endif()

# ========================
# Sources
# ========================
file(GLOB_RECURSE SOFTADASTRA_METADATA_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

set(SOFTADASTRA_METADATA_PLATFORM_SOURCES)

if(UNIX AND NOT APPLE)
    file(GLOB_RECURSE SOFTADASTRA_METADATA_LINUX_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/platform/linux/*.cpp"
    )
    list(APPEND SOFTADASTRA_METADATA_PLATFORM_SOURCES
        ${SOFTADASTRA_METADATA_LINUX_SOURCES}
    )
endif()

if(APPLE)
    file(GLOB_RECURSE SOFTADASTRA_METADATA_MAC_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/platform/mac/*.cpp"
    )
    list(APPEND SOFTADASTRA_METADATA_PLATFORM_SOURCES
        ${SOFTADASTRA_METADATA_MAC_SOURCES}
    )
endif()

if(WIN32)
    file(GLOB_RECURSE SOFTADASTRA_METADATA_WINDOWS_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/*.cpp"
    )
    list(APPEND SOFTADASTRA_METADATA_PLATFORM_SOURCES
        ${SOFTADASTRA_METADATA_WINDOWS_SOURCES}
    )
endif()

# ========================
# Library
# ========================
add_library(softadastra_metadata
    ${SOFTADASTRA_METADATA_SOURCES}
    ${SOFTADASTRA_METADATA_PLATFORM_SOURCES}
)

add_library(softadastra::metadata ALIAS softadastra_metadata)

target_compile_features(softadastra_metadata PUBLIC cxx_std_20)

target_link_libraries(softadastra_metadata
    PUBLIC
        ${SOFTADASTRA_METADATA_DISCOVERY_TARGET}
)

# ========================
# Include directories
# ========================
target_include_directories(softadastra_metadata
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# ========================
# Compiler options
# ========================
if(MSVC)
    target_compile_options(softadastra_metadata PRIVATE /W4 /permissive-)
else()
    target_compile_options(softadastra_metadata PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ========================
# Platform definitions
# ========================
if(UNIX AND NOT APPLE)
    target_compile_definitions(softadastra_metadata PRIVATE SOFTADASTRA_METADATA_LINUX)
elseif(APPLE)
    target_compile_definitions(softadastra_metadata PRIVATE SOFTADASTRA_METADATA_MAC)
elseif(WIN32)
    target_compile_definitions(softadastra_metadata PRIVATE SOFTADASTRA_METADATA_WINDOWS)
endif()

# ========================
# Target properties
# ========================
set_target_properties(softadastra_metadata PROPERTIES
    OUTPUT_NAME softadastra_metadata
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    EXPORT_NAME metadata
)

# ========================
# Examples
# ========================
if(SOFTADASTRA_METADATA_BUILD_EXAMPLES AND NOT SOFTADASTRA_UMBRELLA_BUILD)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
        add_subdirectory(examples)
    else()
        message(STATUS "[metadata] examples/ missing CMakeLists.txt -> skipping")
    endif()
endif()

# ========================
# Tests
# ========================
if(SOFTADASTRA_METADATA_BUILD_TESTS)
    include(CTest)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
        add_subdirectory(tests)
    else()
        message(STATUS "[metadata] tests/ missing CMakeLists.txt -> skipping")
    endif()
endif()

# ========================
# Summary
# ========================
message(STATUS "------------------------------------------------------")
message(STATUS "softadastra::metadata configured (${PROJECT_VERSION})")
message(STATUS "Discovery target: ${SOFTADASTRA_METADATA_DISCOVERY_TARGET}")
message(STATUS "Include dir:      ${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "Build tests:      ${SOFTADASTRA_METADATA_BUILD_TESTS}")
message(STATUS "Build examples:   ${SOFTADASTRA_METADATA_BUILD_EXAMPLES}")
message(STATUS "Build type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "------------------------------------------------------")
