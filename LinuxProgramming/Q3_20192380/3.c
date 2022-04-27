#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 128

int main(int argc, char *argv[]) {
    char buf[BUFFER_SIZE];
    int fd1, fd2;
    ssize_t size;
    if (argc != 3) {
        fprintf(stderr, "Usage: program <source file> <dest file>\n");
        exit(1);
    }
    fd1 = open(argv[1], O_RDONLY), fd2 = open(argv[2], O_RDWR | O_CREAT, 0666);


    if (!fd1 || !fd2) {
        fprintf(stderr, "File cannot be opened.\n");
        close(fd1);
        close(fd2);
        exit(1);
    }

    while ((size = read(fd1, buf, BUFFER_SIZE)) != 0)
        write(fd2, buf, size);

    close(fd1);
    close(fd2);

    exit(0);
}