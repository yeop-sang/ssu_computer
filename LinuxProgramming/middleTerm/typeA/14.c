#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
     char buf[BUFFER_SIZE];
     char* fname = "ssu_test.txt";
     int fd;

     int cnt = 0;

     fd = open(fname, O_RDONLY);

     dup2(fd, 0);

     while(scanf("%s", buf) != EOF)
         printf("fd scanf : %s\n", buf);

    close(fd);

    exit(0);
}

