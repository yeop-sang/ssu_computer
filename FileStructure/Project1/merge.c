#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define MY_BUFF_SIZE 100

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: program <src1_filename> <src2_filename> <dest_filename>\n");
        return 1;
    }

    char *src1_filename = argv[1], *src2_filename = argv[2], *dest_filename = argv[3];

    int src1_file_description = open(src1_filename, O_RDONLY),
            src2_file_description = open(src2_filename, O_RDONLY),
            dest_file_description = open(dest_filename, O_CREAT | O_WRONLY, 0644);

    if (src1_file_description < 0 || src2_file_description < 0 || dest_file_description < 0) {
        fprintf(stderr, "Have some Trouble in opening files");
        close(src1_file_description);
        close(src2_file_description);
        close(dest_file_description);
        return 1;
    }

    char file_buffer[MY_BUFF_SIZE] = "";
    int read_data_size = 0;

    // copy src1
    while ((read_data_size = read(src1_file_description, file_buffer, MY_BUFF_SIZE)) > 0) {
        write(dest_file_description, file_buffer, read_data_size);
    }

    // copy src2
    while ((read_data_size = read(src2_file_description, file_buffer, MY_BUFF_SIZE)) > 0) {
        write(dest_file_description, file_buffer, read_data_size);
    }

    close(src1_file_description);
    close(src2_file_description);
    close(dest_file_description);

    return 0;
}