cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(libcalc_headers INTERFACE)
target_include_directories(libcalc_headers SYSTEM INTERFACE include)

add_library(libcalc INTERFACE)
target_sources(libcalc INTERFACE libcalc.c decimal.c arithmetic.c)

target_link_libraries(libcalc INTERFACE libcalc_headers)
