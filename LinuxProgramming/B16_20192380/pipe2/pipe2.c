#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(void) {
    char buf[BUFFER_SIZE];
    int pid;
    int pipe_fd[2];

    pipe(pipe_fd);

    if((pid = fork()) < 0) {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    // 부모 process인 경우
    else if(pid > 0) {
        printf(" Parent: writing to the pipe \n");
        // pipe의 쓰기에 OSLAB 씀
        write(pipe_fd[1], "OSLAB", 6);
        printf(" PARENT: waiting \n");
        // 자식 process 대기
        wait(NULL);
    }
    // 자식 process의 경우
    else {
        printf(" CHILD: reading from pipe \n");
        // 부모에서 쓴 거 읽어옴
        read(pipe_fd[0], buf, 6);
        printf(" CHILD: read \"%s\" \n", buf);
        exit(0);
    }

    exit(0);
}
