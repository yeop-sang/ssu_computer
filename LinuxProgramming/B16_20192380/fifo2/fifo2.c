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
#define FIFO_NAME "ssu_fifofile"

int main(void) {
    char buf[BUFFER_SIZE];
    int fd;
    int length;

    // mknod fifo file 생성
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);
    printf("waiting for writers... \n");

    // 읽어올 file 열음
    if((fd = open(FIFO_NAME, O_RDONLY)) < 0) {
        fprintf(stderr, "open error for %s\n", FIFO_NAME);
        exit(1);
    }

    printf("got a writer \n");

    do {
        // 읽어와서 출력
        if((length = read(fd, buf, BUFFER_SIZE)) == -1) {
            fprintf(stderr, "read error\n");
            exit(1);
        }
        else {
            buf[length] = '\0';
            printf("tick: read %d bytes: \"%s\"\n", length, buf);
        }
    } while(length > 0);

    exit(0);
}
