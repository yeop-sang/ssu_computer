#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define WORD_MAX 100

int main() {
    int fd;
    int length = 0, offset = 0, count = 0;
    char *fname = "ssu_test.txt";
    char buf[WORD_MAX][BUFFER_SIZE];
    int i;

    if((fd = open(fname, O_RDONLY)) < 0) {
        fprintf(stderr, "file open error\n");
        exit(1);
    }

    while(1) {
        length = read(fd, buf[count], BUFFER_SIZE);
        if(length <= 0)
           break;
        length = 0;
        while(buf[count][length++] != '\n');
        offset += length;
        count++;
        lseek(fd, offset, SEEK_SET);
    }

    close(fd);

    for(i = 0; i < count; i++)
        printf("%s\n", buf[i]);

    printf("line number : %d\n", count);

    exit(0);
}

