/* sigsuspend - block 시킬 signal을 재설정함과
 *              동시에 캐치할 수 있는 시그널이 도착하거나
 *              SIGINT 가 도착할 때까지 process 를 잠시 block 시키는 action을 atomic 하게 실행
 *
 *              sigprocmask와 pause 사이에 발생할 수 있는 signal 을 처리하는데 사용
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void) {
    sigset_t old_set;
    sigset_t sig_set;

    // 초기화하고 처리할 signal 저장하고 old_set에 기존 인자와 함께 복사
    sigemptyset(&sig_set);
    sigaddset(&sig_set, SIGINT);
    sigprocmask(SIG_BLOCK, &sig_set, &old_set);

    // SIGINT 발생시까지 대기!
    sigsuspend(&old_set);
    exit(0);
}
