#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(void) {
    char buf[BUFFER_SIZE];
    // return 값이 배열로 나옴
    // 0은 읽기 전용, 1은 쓰기 전용
    int pipe_fd[2];
    // pipe -1 이면 에러
    if (pipe(pipe_fd) == -1) {
        fprintf(stderr, "pipe error\n");
        exit(1);
    }

    // 쓰기 descriptor에 작성
    printf("writing to file descriptor #%d \n", pipe_fd[1]);
    write(pipe_fd[1], "OSLAB", 6);

    // 읽기 descriptor에서 읽음
    printf("reading from file descriptor #%d \n", pipe_fd[0]);
    read(pipe_fd[0], buf, 6);
    printf("read \"%s \" \n", buf);
    exit(0);
}