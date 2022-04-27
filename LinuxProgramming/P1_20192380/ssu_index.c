#include <stdio.h>
#include <string.h>
#include "slicing.h"
#include "my_find.h"
#include <sys/time.h>

void prompt() {
    // prompt 출력을 위한 함수
    char inputBuffer[STR_BUFFER_SIZE], keyword[3][STR_BUFFER_SIZE], *cmd;
    while (1) {
        printf("%s> ", STUDENT_ID);
        fgets(inputBuffer, STR_BUFFER_SIZE, stdin);

        // keyword에 입력된 값을 띄어쓰기에 따라 분리해 넣어둠
        slicing_str(inputBuffer, keyword);

        // keyword[0]은 커맨드를 저장해 두는 곳,
        // keyword[1]은 find 의 filename
        // keyword[2]는 find 의 path
        cmd = keyword[0];
        if (!strcmp(cmd, "find")) {
            // 만약 path의 마지막이 '/'가 들어갔다면 일관성을 위해 제거하기로 함.
            if (keyword[2][strlen(keyword[2]) - 1] == '/' && is_dir(keyword[2])) {
                keyword[2][strlen(keyword[2]) - 1] = '\0';
            }
            ssu_find(keyword[1], keyword[2]);
        } else if (!strcmp(cmd, "exit")) {
            // exit이면 종료
            printf("Prompt End\n");
            break;
        } else if (!strcmp(cmd, "help")) {
            // help 출력
            printf("Usage:\n"
                   "\t> find [FILENAME] [PATH]\n"
                   "\t\t>> [INDEX] [OPTION ... ]\n"
                   "\t> help\n"
                   "\t> exit\n"
                   "\n"
                   "\t[OPTIONS ... ]\n"
                   "\t  q : report only when files differ\n"
                   "\t  s : report when two files are the same\n"
                   "\t  i : ignore case differences in file contents\n"
                   "\t  r : recursively compare any subdirectories found\n"
                   "\t  l : print only different lines\n");
        }
        // 이외에는 다시 반복
    }
}

int main() {
    struct timeval startTime, endTime;
    double diffTime;
    gettimeofday(&startTime, NULL);
    prompt();
    gettimeofday(&endTime, NULL);

    diffTime =
            ((double) endTime.tv_sec - startTime.tv_sec) + (((double) endTime.tv_usec - startTime.tv_usec) / 1000000);
    printf("%f s\n", diffTime);
    return 0;
}
