#include "find.h"
#include "hash.h"

int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s [FILE_EXTENSION] [MIN_SIZE] [MAX_SIZE] [TARGET_DIRECTORY]", argv[0]);
        exit(1);
    }

    find_command(argv[1], argv[2], argv[3], argv[4], "md5");

    exit(0);
}
