#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

void ssu_signal_handler(int signo);

jmp_buf jump_buffer;

int main(void) {
    // ctrl + c에 대한 처리를 ssu_signal_handler로 처리함
    signal(SIGINT, ssu_signal_handler);

    while(1) {
        // 점프될 위치 호출
        if(setjmp(jump_buffer) == 0) {
            printf("Hit Ctrl-c at anytime ... \n");
            pause();
        }
    }

    exit(0);
}

void ssu_signal_handler(int signo) {
    char character;

    // 이후에 들어오는 signal (SIGINT 뿐)을 무시한다.
    signal(signo, SIG_IGN);
    printf("Did you hit Ctrl-c?\n"
           "Do you really want to quit? [y/n]"
    );
    character = getchar();

    // y가 들어오면 종료
    if (character == 'y' || character == 'Y')
        exit(0);
    else {
        signal(SIGINT, ssu_signal_handler);
        longjmp(jump_buffer, 1);
    }
}
