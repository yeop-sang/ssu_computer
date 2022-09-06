#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#define DIRECTORY_SIZE MAXNAMELEN

void print_file_type(struct stat* statbuf) {
    char *str;

    switch (statbuf->st_mode & S_IFMT) {
        case S_IFBLK:   
            str = "block special\n"; 
            break;
        case S_IFCHR:
            str="character specal\n";
            break;
        case S_IFDIR:
            str="directory\n";
            break;
        case S_IFIFO:
            str="FIFO\n";
            break;
        case S_IFLNK:
            str="symlink\n";
            break;
        case S_IFREG:
            str="regular file\n";
            break;
        default:
            str="unknown?\n";
            break;
    }

    printf("%s", str);

}

int main(int argc, int **argv) {
    struct dirent *dentry;
    struct stat statbuf;
    DIR *dirp;
    char filename[DIRECTORY_SIZE+1];

    struct dirent **namelist;
    int n = scandir(argv[1], &namelist, NULL, alphasort);

    if(n == -1) {
        fprintf(stderr, "scandir error");
        exit(1);
    }

    while(n--) {
        strcpy(filename, namelist[n]->d_name);
        stat(&statbuf);       
        print_file_type(&statbuf);
        free(namelist[n]);
    }

    exit(0);
}

