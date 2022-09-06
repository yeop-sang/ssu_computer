#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    int f1 = open(argv[1], O_RDONLY);
    int f2 = open(argv[2], O_CREAT|O_RDWR, 0644);
    
    int s = 99999;

    char buf[s];

    while((s = read(f1, buf, s)) > 0) {
        write(f2, buf, s);
    }

    close(f1);
    close(f2);

    exit(0);
}
