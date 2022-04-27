// file 처리와 관련된 함수들을 모아둠
#include "my_file.h"

int is_dir(char *full_path) {
    f_stat fs;
    stat(full_path, &fs);
    return S_ISDIR(fs.st_mode);
}

int cmp_fname_with_path(char *filename, char *cmp_file_path) {
    char temp_fname[STR_BUFFER_SIZE];
    path_to_fname(cmp_file_path, temp_fname);
    return strcmp(filename, temp_fname) == 0;
}

int get_same_file(char *filename, my_file file, char *cmp_file_path, my_file *saved) {
    f_stat fs;
    stat(cmp_file_path, &fs);
    char buffer[STR_BUFFER_SIZE];
    realpath(cmp_file_path, buffer);

    // 원본과 같으면 return 0
    if (!strcmp(buffer, file.path))
        return 0;

    // 파일 이름이 다르거나 크기가 다르면 return 0;
    if (
            !cmp_fname_with_path(filename, cmp_file_path) ||
            fs.st_size != file.st.st_size
            ) {
        return 0;
    }

    saved->st = fs;
    saved->fd = fopen(cmp_file_path, "r");
    realpath(cmp_file_path, saved->path);
    return 1;
}

void print_mode(m) {
    printf(S_ISDIR(m) ? "d" : "-");
    printf((m & S_IRUSR) ? "r" : "-");
    printf((m & S_IWUSR) ? "w" : "-");
    printf((m & S_IXUSR) ? "x" : "-");
    printf((m & S_IRGRP) ? "r" : "-");
    printf((m & S_IWGRP) ? "w" : "-");
    printf((m & S_IXGRP) ? "x" : "-");
    printf((m & S_IROTH) ? "r" : "-");
    printf((m & S_IWOTH) ? "w" : "-");
    printf((m & S_IXOTH) ? "x" : "-");
    printf(" ");
}

void print_file_col() {
    printf("%-6s", "Index");
    printf("%-7s", "Size");
    printf("%-11s", "Mode");
    printf("%-7s", "Blocks");
    printf("%-6s", "Links");
    printf("%-5s", "UID");
    printf("%-5s", "GID");

    printf("%-15s", "Access");
    printf("%-15s", "Change");
    printf("%-15s", "Modify");
    printf("Path\n");
}

void print_file_info(int index, my_file f) {
    f_stat fs = f.st;
    char atime[DATE_BUFF], ctime[DATE_BUFF], mtime[DATE_BUFF];
    struct tm struct_atime, struct_ctime, struct_mtime;

    localtime_r((const time_t *) &fs.st_atimespec, &struct_atime);
    localtime_r((const time_t *) &fs.st_ctimespec, &struct_ctime);
    localtime_r((const time_t *) &fs.st_mtimespec, &struct_mtime);

    strftime(atime, sizeof(atime), "%y-%m-%d %H:%M\0", &struct_atime);
    strftime(ctime, sizeof(ctime), "%y-%m-%d %H:%M\0", &struct_ctime);
    strftime(mtime, sizeof(mtime), "%y-%m-%d %H:%M\0", &struct_mtime);

    printf("%-5d ", index);
    printf("%-6lld ", fs.st_size);
    print_mode(fs.st_mode);
    printf("%-6lld ", fs.st_blocks);
    printf("%-5hu ", fs.st_nlink);
    printf("%-4u ", fs.st_uid);
    printf("%-4u ", fs.st_gid);
    printf("%s ", atime);
    printf("%s ", ctime);
    printf("%s ", mtime);
    printf("%s", f.path);
    printf("\n");
}

void close_files(my_file* found_files, int file_index) {
    for (int i = 0; i < file_index; ++i) {
        fclose(found_files[i].fd);
    }
}
