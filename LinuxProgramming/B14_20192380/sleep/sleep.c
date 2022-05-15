#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void ssu_timestamp(char *str);

int main(void) {
    unsigned int ret;
    ssu_timestamp("before sleep()");
    // 10초간 정지
    // alarm과 sigsuspend로 구현 가능
    ret = sleep(10);
    ssu_timestamp("after sleep()");
    printf("sleep() returned %d\n", ret);
    exit(0);
}

void ssu_timestamp(char *str) {
    time_t time_val;
    time(&time_val);
    printf("%s the time is %s\n", str, ctime(&time_val));
}