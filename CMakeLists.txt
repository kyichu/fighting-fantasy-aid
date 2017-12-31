cmake_minimum_required (VERSION 2.6)
project (fighting-fantasy)

file (GLOB fighting-fantasy_SRC "*.c")

set (CMAKE_C_FLAGS "-g -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable (fighting-fantasy ${fighting-fantasy_SRC})
