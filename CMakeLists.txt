cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(little_owls CXX)

set(EXAMPLE_NAME little_owls)
set(TinyTIFF_DIR)
add_subdirectory(external/TinyTIFF)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/TinyTIFF)

add_library(runsim SHARED
    RunSim.cpp
    Environment.cpp
    Model.cpp
    Individual.cpp
    RandomGenerator.cpp
    Solar.cpp
    Wrapper.cpp
    callModel.cpp
)

find_package(Boost REQUIRED COMPONENTS math_c99 math_c99f math_c99l)

# If Boost was found, include its headers and link to Boost libraries
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    #target_link_libraries(runsim ${Boost_LIBRARIES})
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/TinyTIFF/src)

# Uncomment if the test executable is needed and the source file is available
#add_executable(${EXAMPLE_NAME}
#        external/TinyTIFF/tests/extcmake_tinytiff_test/extcmake_tinytiff_test.cpp
#)
#target_link_libraries(${EXAMPLE_NAME} TinyTIFF)

# Set properties for the shared library if needed
#set_target_properties(runsim PROPERTIES
#    OSX_ARCHITECTURES arm64
#)

