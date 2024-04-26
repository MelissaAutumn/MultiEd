cmake_minimum_required(VERSION 3.21)

project(MultiEdProjectSuite)

##
# Common settings
##
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Remove the lib prefix
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64")

# Defines
if (WIN32)
    # ???
else()
    add_compile_definitions("__LINUX_X86__")
endif()
add_compile_definitions("BUILD_64")
add_compile_definitions("_DEBUG")

# From UTSDK
add_compile_definitions(UNICODE=1)
add_compile_definitions(_UNICODE=1)
add_compile_definitions(_REENTRANT=1)
add_compile_definitions(HAVE_PRAGMA_PACK=1)

# For UT
add_compile_definitions(ENGINE_ARCHITECTURE="amd64")

##
# Projects
##

# Add QT6Shared
add_subdirectory(src/QT6Shared)

# Add MultiEd-bin
add_subdirectory(src/MultiEd)

# Add QT6Drv
add_subdirectory(src/QT6Drv)

# Add QT6Launch (Test project to ensure QT6Drv works for MultiEd)
add_subdirectory(src/QT6Launch)
