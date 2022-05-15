#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void ssu_signal_handler(int signo);

void ssu_timestamp(char *str);

int main(void) {
    struct sigaction sig_act;
    sigset_t blk_set;

    // 모든 signal 추가 후 SIGALRM만 삭제
    sigfillset(&blk_set);
    sigdelset(&blk_set, SIGALRM);
    sigemptyset(&sig_act.sa_mask);

    // SIGALRM 에 대한 handler 로 ssu_signal_handler 이용
    sig_act.sa_flags = 0;
    sig_act.sa_handler = ssu_signal_handler;
    sigaction(SIGALRM, &sig_act, NULL);
    ssu_timestamp("before sigsuspend()");
    alarm(5);

    // SIGALRM이 올때까지 대기
    sigsuspend(&blk_set);
    ssu_timestamp("after sigsuspend()");
    exit(0);
}

void ssu_signal_handler(int signo) {
    printf("in ssu_signal_handler() function\n");
}

void ssu_timestamp(char *str) {
    time_t time_val;
    time(&time_val);
    printf("%s the time is %s\n", str, ctime(&time_val));
}