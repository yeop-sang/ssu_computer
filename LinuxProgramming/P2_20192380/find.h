#ifndef P2_20192380_FIND_H
#define P2_20192380_FIND_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <sys/time.h>
#include "my_structs.h"
#include "tokenizer.h"

void find_dup(char *file_extension, off_t min_size, off_t max_size, char *target_dir,
              void (*file_hasher)(char *result, FILE *file_ptr), int digest_length);

void find_command(char *file_extension, char* min_size_str, char* max_size_str, char *target_dir, char* hash_type);

#endif //P2_20192380_FIND_H
