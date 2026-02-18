cmake_minimum_required(VERSION 3.10)
project(PlayGTAV)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(PlayGTAV WIN32
    PlayGTAV.cpp
    PlayGTAV.rc
)

# Link required libraries (Windows specific)
if(WIN32)
    target_link_libraries(PlayGTAV shell32)
endif()

# Set output directory
set_target_properties(PlayGTAV PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
