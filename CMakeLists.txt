cmake_minimum_required(VERSION 3.10)
project(GTA5launcher)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(GTA5_launcher
    GTA5_launcher.cpp
    GTA5_launcher.rc
)

# Link required libraries (Windows specific)
if(WIN32)
    target_link_libraries(GTA5_launcher shell32)

    # Make sure the RC compiler can find windows.h and verrsrc.h from the Windows SDK
    # We include the source directory so icon.ico is found relative to the .rc file
    set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /I \"${CMAKE_SOURCE_DIR}\" /nologo")

    # Also propagate Windows SDK include paths to the RC compiler
    if(DEFINED CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
        set(_SDK_VER "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")
    elseif(DEFINED ENV{WindowsSDKVersion})
        string(REGEX REPLACE "\\\\$" "" _SDK_VER "$ENV{WindowsSDKVersion}")
    endif()

    if(DEFINED _SDK_VER)
        set(_SDK_INCLUDE "C:/Program Files (x86)/Windows Kits/10/Include/${_SDK_VER}")
        if(EXISTS "${_SDK_INCLUDE}/um")
            set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /I \"${_SDK_INCLUDE}/um\" /I \"${_SDK_INCLUDE}/shared\"")
        endif()
    endif()
    
    # Add verbose output for debugging
    set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /v")
endif()

# Set output directory
set_target_properties(GTA5_launcher PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
