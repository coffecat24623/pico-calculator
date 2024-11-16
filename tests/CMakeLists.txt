cmake_minimum_required(VERSION 3.13)
project(math_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/../libcalc libcalc_lib)

# Make a shared library so python tools can load and run unit tests
add_library(testutils SHARED)
target_link_libraries(testutils libcalc)

add_executable(math_test
	math_test.c
)

target_link_libraries(math_test testutils)

