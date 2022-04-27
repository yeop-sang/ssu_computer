cmake_minimum_required(VERSION 3.16)
project(P2_20192380 C)

set(CMAKE_C_STANDARD 11)

# Help 이름 변경
set(HELP_OUTPUT_NAME "ssu_help")
add_definitions(-DHELP_OUTPUT_NAME=\"${HELP_OUTPUT_NAME}\")

#### 테스트용 변수
#add_definitions(-DDEBUG)
#####

set(FILE_BUFFER_SIZE 1024*16)
set(STUDENT_ID 20192380)
set(PATH_MAX 4096)
set(FILE_NAME_MAX 256)
set(BUFFER_SIZE 1024)
set(MAX_ARGV 5)
set(TIME_LENGTH 20)
set(FILE_MAX_SIZE 5000000000)
set(TRASH_CAN_PATH ./.ssu_sdup_trash)

add_definitions(-DFILE_BUFFER_SIZE=${FILE_BUFFER_SIZE})
add_definitions(-DSTUDENT_ID=${STUDENT_ID})
add_definitions(-DPATH_MAX=${PATH_MAX})
add_definitions(-DFILE_NAME_MAX=${FILE_NAME_MAX})
add_definitions(-DBUFFER_SIZE=${BUFFER_SIZE})
add_definitions(-DMAX_ARGV=${MAX_ARGV})
add_definitions(-DTIME_LENGTH=${TIME_LENGTH})
add_definitions(-DFILE_MAX_SIZE=${FILE_MAX_SIZE})
add_definitions(-DTRASH_CAN_PATH=\"${TRASH_CAN_PATH}\")

add_library(sdup_library SHARED hash.c hash.h find.c find.h my_structs.c my_structs.h tokenizer.c tokenizer.h)
target_link_libraries(sdup_library crypto)

add_executable(fmd5 ssu_find-md5.c)
target_link_libraries(fmd5 PUBLIC sdup_library)

add_executable(fsha1 ssu_find-sha1.c)
target_link_libraries(fsha1 PUBLIC sdup_library)

add_executable(ssu_help ssu_help.c)
set_target_properties(
        ssu_help
        PROPERTIES
            OUTPUT_NAME ${HELP_OUTPUT_NAME}
)

add_executable(ssu_sdup ssu_sdup.c tokenizer.c tokenizer.h)

message("$ENV{LDFLAGS}")
