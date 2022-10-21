cmake_minimum_required(VERSION 3.23)
project(Morpion36 C)

set(CMAKE_C_STANDARD 99)

add_executable(Morpion36 main.c core.c core.h types.h display.c display.h)
