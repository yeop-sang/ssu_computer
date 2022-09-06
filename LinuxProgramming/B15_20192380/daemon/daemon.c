#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

int ssu_daemon_init(void);
int main(void) {
    pid_t pid;

    pid = getpid();
    printf("parent process : %d\n", pid);
    printf("daemon process initialization\n");

    if(ssu_daemon_init() < 0) {
        fprintf(stderr, "ssu_daemon_init failed\n");
        exit(1);
    }
    exit(0);
}

int ssu_daemon_init(void) {
    pid_t pid;
    int fd, maxfd;

    if((pid = fork()) < 0) {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    // 부모 process의 경우!
    else if(pid != 0) {
        exit(0);
    }

    pid = getpid();
    printf("process %d running as daemon\n", pid);
    // 새로운 session 생성, 부모와 독립적으로 작동하게 만든다.
    setsid();

    // terminal I/O signal 무시!
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);

    // 모든 file descriptor를 닫는다.
    maxfd = getdtablesize();
    for (fd = 0; fd < maxfd; ++fd) {
        close(fd);
    }

    // 생성 마스크를 0으로 만들고
    umask(0);
    // root directory 로 이동
    chdir("/");
    // 표준 입출력과 표준 에러를 /dev/null로 재지정하여 모든 라이브러리 루틴을 무효화할 수 있다.
    fd = open("/dev/null", O_RDWR);
    dup(0);
    dup(0);
    return 0;
}
