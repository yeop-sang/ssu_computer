#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    char buf[BUF_SIZE];
    struct stat statbuf_1, statbuf_2;

    if(argc != 3)
    {
        fprintf(stderr, "usage : %s <filename1> <filename2>\n", argv[0]);
        exit(1);
    }

    // 5. stat error handling

    int a, b;
    if((a =lstat(argv[1], &statbuf_1)) || (b =lstat(argv[2], &statbuf_2)))
    {
        fprintf(stderr, "file cannot open\n");
        exit(1);
    }

    // 6. reg file

    if(!S_ISREG(statbuf_1.st_mode) || !S_ISREG(statbuf_2.st_mode))
    {
        fprintf(stderr, "file is not reg\n");
        exit(1);
    }

    // 7. system call

    char cmd[BUF_SIZE*10];

    strcpy(cmd, "diff ");
    strcat(cmd, argv[1]);
    strcat(cmd, " ");
    strcat(cmd, argv[2]);

    system(cmd);

    exit(0);
}
