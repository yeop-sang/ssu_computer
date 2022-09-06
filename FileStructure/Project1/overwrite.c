#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: program <offset> <data> <filename>");
        return 1;
    }

    long offset = strtol(argv[1], NULL, 10);
    char* data = argv[2];
    char* dest_filename = argv[3];

    int dest_file_descriptor = open(dest_filename, O_RDWR);

    if (dest_file_descriptor < 0) {
        fprintf(stderr, "Error opening %s", dest_filename);
        return 1;
    }

    lseek(dest_file_descriptor, offset, SEEK_SET);

    write(dest_file_descriptor, data, strlen(data));

    close(dest_file_descriptor);

    return 0;
}