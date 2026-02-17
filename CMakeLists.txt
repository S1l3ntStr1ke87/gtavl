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
endif()

# Set output directory
set_target_properties(GTA5_launcher PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)