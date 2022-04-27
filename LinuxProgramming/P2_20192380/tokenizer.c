#include "tokenizer.h"

int tokenization(char *string, char *separator, char *argv[]) {
    int argc = 0;
    char *substring = NULL;

    substring = strtok(string, separator);
    while (substring != NULL && argc <= MAX_ARGV) {
        argv[argc++] = substring;
        substring = strtok(NULL, separator);
    }

    return argc;
}
