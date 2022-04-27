

#ifndef P1_20192380_MY_FILE_H
#define P1_20192380_MY_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "slicing.h"

typedef struct stat f_stat;

#define DATE_BUFF 16

typedef struct {
    f_stat st;
    FILE *fd;
    char path[STR_BUFFER_SIZE];
    char name[STR_BUFFER_SIZE];
} my_file;

void print_file_info(int index, my_file f);

void print_file_col();

int is_dir(char *full_path);

int get_same_file(char *filename, my_file file, char *cmp_file_path, my_file *saved);

void close_files(my_file* found_files, int file_num);

#endif //P1_20192380_MY_FILE_H
