#include "ssu_find-md5.h"
#include "ssu_find-sha1.h"
#include "ssu_help.h"

#define STRMAX 10000
#define ST_ID "20192380"

//fsha1 -e * -l ~ -h ~ -d ~/code/dup

int main_tokenize(char *input, char *argv[]) {
    char *ptr = NULL;
    int argc = 0;
    ptr = strtok(input, " ");

    while (ptr != NULL) {
        argv[argc++] = ptr;
        ptr = strtok(NULL, " ");
    }
    return argc;
}

int main(void) {
    if (!get_username())
        exit(1);
    if (new_logger())
        exit(1);
    while (1) {
        char input[STRMAX];
        char *argv[ARGMAX];

        int argc = 0;
        pid_t pid;

        printf("%s> ", ST_ID);
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';

        argc = main_tokenize(input, argv);
        argv[argc] = (char *) 0;

        if (argc == 0)
            continue;

        if (!strcmp(argv[0], "exit"))
            break;

        if (!strcmp(argv[0], "fmd5"))
            fmd5(argc, argv);
        else if (!strcmp(argv[0], "fsha1"))
            fsha1(argc, argv);
            // list 작업
        else if (!strcmp(argv[0], "list"))
            list(argc, argv);
            // trash 작업
        else if (!strcmp(argv[0], "trash"))
            trash(argc, argv);
            // restore 작업
        else if (!strcmp(argv[0], "restore"))
            restore(argc, argv);
        // 이외에 help 출력
        else
            help();
    }

    printf("Prompt End\n");

    close_logger();

    return 0;
}