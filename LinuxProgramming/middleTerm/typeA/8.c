#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

time_t mtime = 0;

void check_file(char* fname) {
    struct stat buf;
    if(stat(fname, &buf) < 0) {
        printf("Warning : ssu_checkfile() error\n");
        exit(1);
    }
    else
        if(buf.st_mtime!= mtime) {
            printf("Warning: %s was modified!\n", fname);
            mtime = buf.st_mtime;
        }
}

int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "argc should be 2");
        exit(1);
    }

    int fd = 0;

    if((fd = open(argv[1], O_RDONLY)) < 0 ){
        fprintf(stderr, "file error");
        exit(1);
    }

    close(fd);

    struct stat buf;
    if(stat(argv[1], &buf) < 0) {
        printf("Warning : ssu_checkfile() error\n");
        exit(1);
    }

    mtime = buf.st_mtime;

    while(1) {
        check_file(argv[1]);
        sleep(2);
    }
}
