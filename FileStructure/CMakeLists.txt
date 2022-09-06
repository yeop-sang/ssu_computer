cmake_minimum_required(VERSION 3.21)
project(FileStructure C)

set(CMAKE_C_STANDARD 99)

add_executable(FileStructure main.c Project1/copy.c)

add_executable(Project1_copy Project1/copy.c)
add_executable(Project1_read Project1/read.c)

add_executable(Project1_overwrite Project1/overwrite.c)
add_executable(Project1_insert Project1/insert.c)
add_executable(Project1_delete Project1/delete.c)