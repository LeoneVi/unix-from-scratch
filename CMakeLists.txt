cmake_minimum_required(VERSION 3.28)
project(unix_tools C)

set(CMAKE_C_STANDARD 17)

add_executable(unix_tools src/tools/cat.c)
