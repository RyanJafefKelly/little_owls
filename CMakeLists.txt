project(little_owls CXX)
cmake_minimum_required(VERSION 3.0)

set(EXAMPLE_NAME little_owls)
set(TinyTIFF_DIR)

find_package(TinyTIFF REQUIRED)

add_executable(${EXAMPLE_NAME}
	extcmake_tinytiff_test.cpp
)
target_link_libraries(${EXAMPLE_NAME} TinyTIFF)