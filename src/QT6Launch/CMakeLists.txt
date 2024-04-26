cmake_minimum_required(VERSION 3.21)
project(QT6Launch)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

# Temp until we have QT6Drv working
find_package(SDL2 REQUIRED)


# Setup some links to shared libraries
add_library(Core SHARED IMPORTED)
add_library(Engine SHARED IMPORTED)
add_library(SDLDrv SHARED IMPORTED)

add_executable(QT6Launch Src/main.cpp)


if (WIN32)
    set_target_properties(Core PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System/Core.so")
    set_target_properties(Engine PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System/Engine.so")
else()
    set_target_properties(Core PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64/Core.so")
    set_target_properties(Engine PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64/Engine.so")
    set_target_properties(QT6Shared PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64/QT6Shared.so")
    set_target_properties(QT6Drv PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64/QT6Drv.so")
    set_target_properties(SDLDrv PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64/SDLDrv.so")
endif()

# Includes
target_include_directories(Core INTERFACE "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/Core/Inc/")
target_include_directories(Engine INTERFACE "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/Engine/Inc/")
target_include_directories(SDLDrv INTERFACE "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/SDLDrv/Inc/")
target_include_directories(QT6Drv INTERFACE "${CMAKE_SOURCE_DIR}/src/QT6Drv/Inc")
target_include_directories(QT6Shared INTERFACE "${CMAKE_SOURCE_DIR}/src/QT6Shared")
# End

target_link_libraries(QT6Launch
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Core
        Engine
        QT6Shared
        #QT6Drv
        SDLDrv
        SDL2
)

set_target_properties(QT6Launch PROPERTIES COMPILE_FLAGS "-fpermissive" LINK_FLAGS "--verbose -Wl,-rpath,$ORIGIN" OUTPUT_NAME "QT6Launch")
