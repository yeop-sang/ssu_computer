#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "tokenizer.h"

#define PATH_SIZE BUFFER_SIZE*2

int main() {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    int argc, process_status;
    pid_t process_id;
    char input[BUFFER_SIZE];
    char *argv[MAX_ARGV + 1];

    while (1) {
        // 프롬프터 출력
        printf("%d> ", STUDENT_ID);
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';
        // 입력된 값 토큰화
        argc = tokenization(input, " ", argv);
        // exec에서 마지막인자로 NULL을 필요로 함!
        argv[argc] = NULL;

        if (argc == 0) {
            continue;
        }

        if (!strcmp(argv[0], "fmd5") || !strcmp(argv[0], "fsha1")) {
            // fmd5 나 fsha1 이 들어오면 해당 프로그램을 실행함!
            if ((process_id = fork()) == 0) {
                // 자식 프로세스에서 fmd5 나 fsha1 실행
                if (execv(argv[0], argv) < 0) {
                    fprintf(stderr, "%s error\n", argv[0]);
                    exit(1);
                }
                // 자식 프로세스 종료
                exit(0);
            } else {
                // 부모 프로세스에서는 자식 프로세스가 종료될 때까지 대기
                if (wait(&process_status) != process_id) {
                    fprintf(stderr, "%s wait error\n", argv[0]);
                    exit(1);
                }
            }
        } else if (!strcmp(argv[0], "exit")) {
            // 프롬프터 종료
            printf("Prompt End");
            break;
        } else {
            // exit, fmd5, fsha1이 아닌 값은 모두 help를 실행시킴
            // mac에서는 help라는 이름으로 실행 파일을 생성하는데 문제가 있어 전처리기로 처리
            if ((process_id = fork()) == 0) {
                if (execv(HELP_OUTPUT_NAME, argv) < 0) {
                    fprintf(stderr, "%s error\n", HELP_OUTPUT_NAME);
                    exit(1);
                }
                exit(0);
            } else {
                if (wait(&process_status) != process_id) {
                    fprintf(stderr, "wait error\n");
                    exit(1);
                }
            }
        }
    }
    gettimeofday(&end_time, NULL);

    end_time.tv_sec -= start_time.tv_sec;
    if (end_time.tv_usec < start_time.tv_usec) {
        end_time.tv_sec--;
        end_time.tv_sec += (long) 1e6;
    }
    end_time.tv_usec -= start_time.tv_usec;
    printf("Runtime: %ld:%06d(sec:usec)\n", end_time.tv_sec, end_time.tv_usec);
}
