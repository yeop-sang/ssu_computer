cmake_minimum_required(VERSION 3.22)
project(Project3 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(
        Project3
        devicedriver.h
        ftl.c
        main.c
)