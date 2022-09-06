#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: program <offset> <remove bytes> <filename>");
        exit(1);
    }

    long offset = strtol(argv[1], NULL, 10);
    long remove_byte = strtol(argv[2], NULL, 10);
    char *filename = argv[3];

    if (remove_byte < 0) {
            // 왼쪽을 삭제해야할 때는 삭제할 데이터만큼 뒤에서 시작해야함!
            long temp_offset = offset + remove_byte;
            if (temp_offset < 0) {
                // 만약 삭제 시작점이 음수이면, 0부터 offset까지만 삭제하면 됨!
                remove_byte = offset;
                offset = 0;
            } else {
                // 삭제할 데이터만큼 앞으로 offset을 당겨준다.
                offset = temp_offset;
                // 항상 remove_byte는 양수여야 함으로 양수로 바꿔준다.
                remove_byte = -remove_byte;
            }
        }

    int dest_file_descriptor = open(filename, O_RDWR);

    if (dest_file_descriptor < 0) {
        fprintf(stderr, "Error opening %s", filename);
        exit(1);
    }

    char file_buffer[remove_byte + 1];
    long input_size = remove_byte;
    long temp_input_size = 0, end_offset;

    if (offset > (end_offset = lseek(dest_file_descriptor, 0, SEEK_END))) {
        close(dest_file_descriptor);
        exit(1);
    } else if (offset + remove_byte > end_offset) {
        ftruncate(dest_file_descriptor, offset);
        exit(0);
    }

    memset(file_buffer, 0, remove_byte + 1);

    lseek(dest_file_descriptor, offset + remove_byte, SEEK_SET);

    while (1) {
        write(dest_file_descriptor, file_buffer, input_size);

        temp_input_size = read(dest_file_descriptor, file_buffer, input_size);

        if (!file_buffer[0]) {
            ftruncate(dest_file_descriptor, lseek(dest_file_descriptor, -temp_input_size, SEEK_CUR));
            break;
        }
        lseek(dest_file_descriptor, -input_size - temp_input_size, SEEK_CUR);

        input_size = temp_input_size;
    }

    close(dest_file_descriptor);

    exit(0);
}

