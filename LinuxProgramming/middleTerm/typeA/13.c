#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define STDERR 2
#define BUFLEN 512

void compile_program(char* filename);
void redirection(char* command, int new, int old);

int main(int argc, char** argv) {
    char buf[BUFLEN];
    if(argc < 2){
        fprintf(stderr, "usage: %s <file1>\n", argv[0]);
        exit(1);
    }

    compile_program(argv[1]);

    exit(0);
}

void compile_program(char* filename) {
    char question_name[BUFLEN];
    char command[BUFLEN];
    char tmp[BUFLEN];

    int fd;
    int size;

    strcpy(question_name, filename);
    question_name[strlen(filename) - strlen(strrchr(filename, ','))] = '\0';
    
    // na, da
    sprintf(tmp, "%s.exe", question_name);
    sprintf(command, "gcc -o %s %s -lpthread", tmp, filename);

    // ga 
    sprintf(tmp, "%s_error.txt", question_name);
    if(fd = open(tmp, O_CREAT | S_IRWXU | S_IRWXG | S_IRWXO)) {
        fprintf(stderr, "%s cannot create\n", tmp);
        exit(1);
    }

    redirection(command, fd, STDERR);

    size = lseek(fd, 0, SEEK_END);

    // delete
    if(size < 0) {
        sprintf(command, "rm %s_error.txt", question_name);
        unlink(tmp);
    }

    close(fd);
}

void redirection(char *command, int new, int old) {
    int saved_fd;

    dup2(old, saved_fd);

    dup2(new, old);

    system(command);

    dup2(saved_fd, old);
    close(saved_fd);

}

