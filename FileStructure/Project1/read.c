#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: program <offset> <print bytes> <filename>\n");
        return 1;
    }

    char *filename = argv[3];

    int file_descriptor = open(filename, O_RDONLY);

    if (file_descriptor < 0) {
        fprintf(stderr, "Error opening %s\n", filename);
        return 1;
    }

    long offset, print_byte;
    offset = strtol(argv[1], NULL, 10);
    print_byte = strtol(argv[2], NULL, 10);

    // byte 수가 0이면 종료한다.
    if (print_byte == 0)
        return 0;

    if (print_byte < 0) {
        // 왼쪽을 출력해야할 때는 검색할 데이터만큼 뒤에서 시작해야함!
        long temp_offset = offset + print_byte;
        if (temp_offset < 0) {
            // 만약 출력 시작점이 음수이면, 0부터 offset까지만 출력하면 됨!
            print_byte = offset;
            offset = 0;
        } else {
            // 출력할 데이터만큼 앞으로 offset을 당겨준다.
            offset = temp_offset;
            // 항상 print_byte는 양수여야 함으로 양수로 바꿔준다.
            print_byte = -print_byte;
        }
    }

    char file_buffer[print_byte + 1];
    memset(file_buffer, 0, print_byte + 1);

    lseek(file_descriptor,offset,SEEK_SET);
    read(file_descriptor, file_buffer, print_byte);

    write(1, file_buffer, print_byte);

    close(file_descriptor);

    return 0;
}