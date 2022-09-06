#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: program <offset> <data> <filename>");
        exit(1);
    }

    long offset = strtol(argv[1], NULL, 10);
    char *data = argv[2];
    char *filename = argv[3];

    int dest_file_descriptor = open(filename, O_RDWR);

    if (dest_file_descriptor < 0) {
        fprintf(stderr, "Error opening %s", filename);
        exit(1);
    }

    int input_data_size[2] = {(int) strlen(data), 0};
    char file_buffer[2][input_data_size[0] + 1];

    memset(file_buffer[1], 0, input_data_size[0] + 1);

    strcpy(file_buffer[0], data);

    // 오프셋만큼 이동
    if (offset <= lseek(dest_file_descriptor, 0, SEEK_END)) {
        lseek(dest_file_descriptor, offset, SEEK_SET);
    }

    int i = 0;

    while (1) {
        // 둘 다 0일때 멈춤
        if (!input_data_size[0] && !input_data_size[1]) break;

        input_data_size[!i] = read(dest_file_descriptor, file_buffer[!i], input_data_size[i]);
        lseek(dest_file_descriptor, -input_data_size[!i], SEEK_CUR);
        write(dest_file_descriptor, file_buffer[i], input_data_size[i]);

        i = !i;
    }

    return 0;
}
