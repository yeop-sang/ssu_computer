#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("abort terminate this program\n");

    // 비정상적으로 종료
    abort();

    // 이 아래는 실행 x

    printf("this line is never reached\n");

    exit(0);
}