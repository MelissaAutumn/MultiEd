cmake_minimum_required(VERSION 3.21)
project(MultiEd)

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

find_package(SDL2 REQUIRED)

add_executable(MultiEd main.cpp
        components/SideBar/ModeBar.h
        components/IComponent.h
        components/SideBar/BaseBar.h
        components/SideBar/ClipBar.h
        components/SideBar/BrushBar.h
        components/SideBar/CSGBar.h
        components/SideBar/MiscBar.h
        components/SideBar/OrderedSideBar.h
        components/Menu/FileMenu.h
        components/Menu/EditMenu.h
        components/Menu/BaseMenu.h
        components/Menu/ViewMenu.h
        components/Menu/BrushMenu.h
        components/Menu/ActorMenu.h
        components/Menu/BuildMenu.h
        components/Menu/ToolsMenu.h
        components/Menu/HelpMenu.h
        components/ToolBar/ToolBar.h
        components/Viewport/Viewport.h
        components/MultiEdWindow.h
        services/EditorAPI.h
        components/MultiEdWindow.cpp
        ue/UnrealGlue.cpp
        ue/UnrealGlue.h
        services/Services.h
        components/Menu/FileMenu.cpp
        services/HelperAPI.h
        services/HelperAPI.cpp
        components/Viewport/ViewportSplitterContainer.h
        components/Viewport/ViewportSplitterContainer.cpp
        components/Viewport/ViewportSplitter.h
        services/EditorAPI.cpp
        helpers/SDLHelper.cpp
        helpers/SDLHelper.h
        helpers/Defines.h
        helpers/Defines.cpp
        components/Viewport/ViewportButton.h
        components/Viewport/ViewportAction.h
        components/SideBar/SidebarButton.h
        components/BrushProperties/BrushProperties.h
        components/LogWindow/LogWindow.h
        helpers/UnrealHelper.h
        helpers/UnrealHelper.cpp
        components/LogWindow/LogWindow.cpp
        components/SideBar/BrushBar.cpp
        components/BrushProperties/BrushProperties.cpp
        components/BrushProperties/TreeProperty.h
        components/Preferences/Preferences.cpp
        components/Preferences/Preferences.h
        # From SDLLaunch
        FFeedbackContextSDL.h
        FOutputDeviceSDLError.h
        )

# Setup some links to shared libraries
add_library(Core SHARED IMPORTED)
add_library(Engine SHARED IMPORTED)
add_library(Editor SHARED IMPORTED)
add_library(ALAudio SHARED IMPORTED)
add_library(SDLDrv SHARED IMPORTED)

if (WIN32)
set_target_properties(Core PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System/Core.dll")
set_target_properties(Engine PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System/Engine.dll")
set_target_properties(Editor PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System/Editor.dll")
set_target_properties(ALAudio PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System/ALAudio.dll")
set_target_properties(SDLDrv PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System/SDLDrv.dll")
else()
set_target_properties(Core PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64/Core.so")
set_target_properties(Engine PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64/Engine.so")
set_target_properties(Editor PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64/Editor.so")
set_target_properties(ALAudio PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64/ALAudio.so")
set_target_properties(SDLDrv PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64/SDLDrv.so")
endif()

# Includes
target_include_directories(Core INTERFACE "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/Core/Inc/")
target_include_directories(Engine INTERFACE "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/Engine/Inc/")
target_include_directories(Editor INTERFACE "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/Editor/Inc/")
target_include_directories(Editor INTERFACE "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/Editor/Src/")  # Do I need this or was this a typo..
target_include_directories(ALAudio INTERFACE "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/ALAudio/Inc/")
target_include_directories(SDLDrv INTERFACE "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/SDLDrv/Inc/")

# End


target_link_libraries(MultiEd
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Core
        Engine
        Editor
        ALAudio
        SDLDrv
        SDL2
        )

set_target_properties(MultiEd PROPERTIES COMPILE_FLAGS "-fpermissive" LINK_FLAGS "--verbose -Wl,-rpath,$ORIGIN" OUTPUT_NAME "MultiEd-bin" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/libs/OldUnrealSDK/System64")
