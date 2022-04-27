#include <stdio.h>

int main(int argc, char** argv) {
    printf("Usage:\n"
           "\t> fmd5/fsha1 [FILE_EXTENSION] [MINSIZE] [MAXSIZE] [TARGET_DIRECTORY]\n"
           "\t\t>> [SET_INDEX] [OPTION ... ]\n"
           "\t\t   [OPTION ... ]\n"
           "\t\t   d [LIST_IDX] : delete [LIST_IDX] file\n"
           "\t\t   i : ask for confirmation before delete\n"
           "\t\t   f : force delete except the recently modified file\n"
           "\t\t   t : force move to Trash except the recently modified file\n"
           "\t> help\n"
           "\t> exit\n");
    return 0;
}
