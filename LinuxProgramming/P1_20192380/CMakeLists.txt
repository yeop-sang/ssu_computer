cmake_minimum_required(VERSION 3.21)
project(P1_20192380 C)

set(PROJECT_NAME ssu_index)
set(CMAKE_C_STANDARD 11)
set(STUDENT_ID \"20192380\")
set(FILE_BUFFER_SIZE 1000)
set(STR_BUFFER_SIZE 1024)

#add_definitions(-DSTUDENT_ID=${STUDENT_ID} -DSTR_BUFFER_SIZE=${STR_BUFFER_SIZE} -DFILE_BUFFER_SIZE=${FILE_BUFFER_SIZE} -DDEBUG=1)
add_definitions(-DSTUDENT_ID=${STUDENT_ID} -DSTR_BUFFER_SIZE=${STR_BUFFER_SIZE} -DFILE_BUFFER_SIZE=${FILE_BUFFER_SIZE})
add_executable(${PROJECT_NAME} ssu_index.c slicing.c my_find.c my_file.c diff.c diff.h)
