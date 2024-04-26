cmake_minimum_required(VERSION 3.21)
project(QT6Shared)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

# Setup some links to shared libraries
add_library(Core SHARED IMPORTED)
add_library(Engine SHARED IMPORTED)
add_library(QT6Shared SHARED
        qt/LogWindow/LogWindow.cpp
        qt/LogWindow/LogWindow.h
        ue/Launch.cpp
        ue/Launch.h
        ue/LogWindow.h
)

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


target_link_libraries(QT6Shared
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Core
        Engine
)

set_target_properties(QT6Shared PROPERTIES COMPILE_FLAGS "-fpermissive" LINK_FLAGS "--verbose -Wl" OUTPUT_NAME "QT6Shared")
