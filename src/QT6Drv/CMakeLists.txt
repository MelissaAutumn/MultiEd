cmake_minimum_required(VERSION 3.21)
project(QT6Drv)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

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

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

# Setup some links to shared libraries
add_library(Core SHARED IMPORTED)
add_library(Engine SHARED IMPORTED)
add_library(QT6Drv SHARED
        Inc/QT6Drv.h
        Src/QT6Client.cpp
        Src/QT6Drv.cpp)



if (WIN32)
    set_target_properties(Core PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System/Core.so")
    set_target_properties(Engine PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System/Engine.so")
else()
    set_target_properties(Core PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64/Core.so")
    set_target_properties(Engine PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64/Engine.so")
endif()

# Includes
target_include_directories(Core INTERFACE "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/Core/Inc/")
target_include_directories(Engine INTERFACE "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/Engine/Inc/")

# End


target_link_libraries(QT6Drv
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Core
        Engine
)

set_target_properties(QT6Drv PROPERTIES COMPILE_FLAGS "-fpermissive" LINK_FLAGS "--verbose -Wl,$ORIGIN" OUTPUT_NAME "QT6Drv" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../System64")
