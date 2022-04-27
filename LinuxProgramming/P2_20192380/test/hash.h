#ifndef P2_20192380_HASH_H
#define P2_20192380_HASH_H

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hash_file_by_md5(char result[MD5_DIGEST_LENGTH], FILE *file_ptr);

void hash_file_by_sha1(char result[SHA_DIGEST_LENGTH], FILE *file_ptr);

#endif //P2_20192380_HASH_H
