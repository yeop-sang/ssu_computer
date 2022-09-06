cmake_minimum_required(VERSION 3.22)
project(P3_20192380 C)

set(CMAKE_C_STANDARD 99)

set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl@3")
find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    message(STATUS "OpenSSL Found: ${OPENSSL_VERSION}")
    message(STATUS "OpenSSL Include: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "OpenSSL Libraries: ${OPENSSL_LIBRARIES}")

endif ()
include_directories("/usr/local/opt/openssl@3/include")

add_executable(ex ssu_sfinder.c ssu_sfinder.h ssu_help.h ssu_find-md5.h ssu_find-sha1.h)
target_link_libraries(ex "/usr/local/opt/openssl@3/lib/libcrypto.a")