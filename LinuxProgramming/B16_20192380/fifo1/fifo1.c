#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
// fifo 와 관련된 환경 변수 저장
#define FILE_MODE S_IWUSR|S_IRUSR|S_IWGRP|S_IRGRP|S_IWOTH|S_IROTH
#define FIFO_NAME "ssu_fifofile"

int main(void) {
    char buf[BUFFER_SIZE];
    int fd;
    int length;

    // fifo file 생성
    mkfifo(FIFO_NAME, FILE_MODE);
    printf("waiting for readers... \n");

    // 파일 읽고 오류 처리
    if((fd = open(FIFO_NAME, O_WRONLY)) < 0) {
        fprintf(stderr, "open error for %s\n", FIFO_NAME);
        exit(1);
    }

    printf("got a reader--type some stuff \n");

    // stdin이 비어 있을때까지 buf에 값을 읽어옴
    while(fgets(buf, BUFFER_SIZE, stdin), !feof(stdin)) {
        if((length = write(fd, buf, strlen(buf) - 1)) == -1) {
            fprintf(stderr, "write error\n");
            exit(1);
        }
        else
            printf("speak: wrote %d bytes \n", length);
    }

    exit(0);
}
