#include <stdio.h>
#include <stdlib.h>

int main() {
    char *fname = "ssu_test.txt";
    char *mode = "r";

    if (fopen(fname, mode) == NULL) {
        fprintf(stderr, "fopen error for %s\n", fname);
        exit(1);
    } else
        printf("Success!\nFilename: <%s>, mod: <%s>\n", fname, mode);

    exit(0);
}