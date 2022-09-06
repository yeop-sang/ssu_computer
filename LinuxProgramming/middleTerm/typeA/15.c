#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>

#define TIME_STRLEN 26

struct stat statbuf;

void ssu_accmodtime(char* fname);

int main(int argc, char *argv[]) {
    time_t acc_time;
    time_t mod_time;

    if(argc != 2) {
        fprintf(stderr, "Usage : %s, <file>\n", argv[0]);
        exit(1);
    }

    if(stat(argv[1], &statbuf) < 0) {
        fprintf(stderr, "stat() error for %s\n", argv[0]);
        exit(1);
    }

    ssu_accmodtime(argv[1]);

    exit(0);
}

void ssu_accmodtime(char* fname) {
    char a_time[TIME_STRLEN] = {};

    strncpy(a_time, ctime(&statbuf.st_atime), strlen(ctime(&statbuf.st_atime)) - 1);

    printf("%s accessed : %s modified : %s", fname, a_time,ctime(&statbuf.st_mtime));

    exit(0);
}

