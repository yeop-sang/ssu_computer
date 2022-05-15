#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

static void ssu_func(int signo);

void ssu_print_mask(const char *str);

int main(void) {
    sigset_t new_mask, old_mask, wait_mask;

    ssu_print_mask("program start: ");

    if (signal(SIGINT, ssu_func) == SIG_ERR) {
        fprintf(stderr, "signal(SIGINT) error\n");
        exit(1);
    }

    // wait mask에 SIGUSR1, new mask에 SIGNIT 할당
    sigemptyset(&wait_mask);
    sigaddset(&wait_mask, SIGUSR1);
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGINT);

    // signal block 처리 및 들어왔던 new_mask 를 old_mask 에 저장
    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
        fprintf(stderr, "sigprocmask() error\n");
        exit(1);
    }

    // signal block 해제
    if(sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        fprintf(stderr, "SIG_SETMASK() error\n");
        exit(1);
    }

    ssu_print_mask("program exit: ");
    exit(0);
}

void ssu_print_mask(const char *str) {
    sigset_t sig_set;
    int err_num;

    err_num = errno;

    if(sigprocmask(0, NULL, &sig_set) < 0) {
        fprintf(stderr, "sigprocmask() error \n");
        exit(1);
    }

    printf("%s", str);

    if(sigismember(&sig_set, SIGINT))
        printf("SIGINT ");

    if (sigismember(&sig_set, SIGQUIT))
        printf("SIGQUIT ");

    if(sigismember(&sig_set, SIGUSR1))
        printf("SIGUSR1 ");

    if(sigismember(&sig_set, SIGALRM))
        printf("SIGALRM ");

    printf("\n");
    errno = err_num;
}

static void ssu_func(int signo) {
    ssu_print_mask("\nin ssu_func: ");
}
