#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BYTE_LIMIT 10

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Usage: ./program <source file> <destination file>");
        exit(1);
    }

    char *src_file_name = argv[1];
    char *dest_file_name = argv[2];

    printf("%s %s\n", src_file_name, dest_file_name);

    int src_file_descriptor = open(src_file_name, O_RDONLY),
            dest_file_descriptor = open(dest_file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);

    if (src_file_descriptor < 0) {
        fprintf((stderr), "source file(%s) cannot open", src_file_name);
        return 1;
    }

    if (dest_file_descriptor < 0) {
        fprintf((stderr), "destination file(%s) cannot open", dest_file_name);
        return 1;
    }

    // debugging을 위해서 맨뒤에 '\0' 추가
    char file_buffer[BYTE_LIMIT + 1] = "";

    // 읽어온 바이트를 저장
    ssize_t read_byte = 0;

    while ((read_byte = read(src_file_descriptor, file_buffer, BYTE_LIMIT)) > 0) {

        // 읽어온 바이트만큼 파일에 적는다.
        if (write(dest_file_descriptor, file_buffer, read_byte) < 0) {
            // 만약 0글자 이내로 썼다고 알려지면 오류가 난 것
            fprintf((stderr), "destination file(%s) cannot write\n", dest_file_name);
            break;
        }
    }

    close(src_file_descriptor);
    close(dest_file_descriptor);

    exit(0);
}
