#include "hash.h"

void stringfy_hash_data(char *result, unsigned char *hash_data, int digest_length) {
    for (int i = 0; i < digest_length; ++i) {
        sprintf(result + (i * 2), "%02x", hash_data[i]);
    }
}

void hash_file_by_md5(char result[MD5_DIGEST_LENGTH * 2 + 1], FILE *file_ptr) {
    MD5_CTX c;
    ssize_t read_byte;
    unsigned char buf[FILE_BUFFER_SIZE], md[MD5_DIGEST_LENGTH];
    int file_descriptor = fileno(file_ptr);

    MD5_Init(&c);
    while (1) {
        read_byte = read(file_descriptor, buf, FILE_BUFFER_SIZE);
        if (read_byte <= 0) break;
        MD5_Update(&c, buf, read_byte);
    }

    MD5_Final(&(md[0]), &c);
    stringfy_hash_data(result, md, MD5_DIGEST_LENGTH);
}

void hash_file_by_sha1(char result[SHA_DIGEST_LENGTH * 2 + 1], FILE *file_ptr) {
    SHA_CTX c;
    ssize_t read_byte;
    unsigned char buf[FILE_BUFFER_SIZE], sha[SHA_DIGEST_LENGTH];
    int file_descriptor = fileno(file_ptr);

    SHA1_Init(&c);
    while (1) {
        read_byte = read(file_descriptor, buf, FILE_BUFFER_SIZE);
        if (read_byte <= 0) break;
        SHA1_Update(&c, buf, read_byte);
    }

    SHA1_Final(&(sha[0]), &c);
    stringfy_hash_data(result, sha, SHA_DIGEST_LENGTH);
}
