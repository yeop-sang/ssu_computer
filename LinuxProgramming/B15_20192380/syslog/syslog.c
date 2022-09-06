#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>

int ssu_daemon_init(void);

int main(void) {
    printf("daemon process initialization\n");

    if(ssu_daemon_init() < 0) {
        fprintf(stderr, "ssu_daemon_init failed\n");
        exit(1);
    }

    while(1) {
        // line printer system에서 lpd라는 구분자로 각 메세지마다 pid 기록
        openlog("lpd", LOG_PID, LOG_LPR);
        // error log 기록
        syslog(LOG_ERR, "open failed lpd %m");
        closelog();
        sleep(5);
    }

    exit(0);
}

int ssu_daemon_init(void) {
    pid_t pid;
    int fd,maxfd;

    if((pid = fork()) < 0) {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    // 부모 인자의 경우 정상 종료
    else if(pid != 0)
        exit(0);

    pid = getpid();
    printf("process %d running as daemon\n", pid);
    // daemon 동작을 위해 session을 해당 pid를 대표로 하나 만듦
    setsid();
    // terminal I/O signal 무시
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    // fd 종료를 위해 fd 최대값 구함
    maxfd = getdtablesize();

    for (fd = 0; fd < maxfd; ++fd) {
        close(fd);
    }

    umask(0);
    chdir("/");
    fd = open("/dev/null",O_RDWR);
    dup(0);
    dup(0);
    return 0;
}
