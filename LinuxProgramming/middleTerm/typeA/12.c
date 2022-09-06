#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    char *fname = "ssu_test.txt";
    char ch;
    int count = 0;

    if((fd = open(fname, O_RDONLY)) < 0) {
        fprintf(stderr, "file error");
        exit(1);
    }

    while(read(fd, &ch, 1)) {
        if(ch == 'M' || ch == 'm')
            count++;
    }

    printf("m + M = %d\n", count);


    exit(0);
}

