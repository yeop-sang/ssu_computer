#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(void) {
    sigset_t set;

    // sigset을 빈 집합으로 만들어준다.
    sigemptyset(&set);
    // sigint signal을 sigset에 추가
    sigaddset(&set, SIGINT);

    switch (sigismember(&set, SIGINT)) {
        case 1:
            // 실행되는 함수에 영향을 받는다.
            printf("SIGINT is included.\n")
    }
}
