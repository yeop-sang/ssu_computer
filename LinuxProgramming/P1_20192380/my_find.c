// find 명령어를 위한 함수들
#include "my_find.h"

void find_files(char *filename, char *root_path, char *search_path, my_file *found_files, int *file_index) {
    struct dirent **namelist;
    char temp_str[STR_BUFFER_SIZE] = {};
    int count;
    int idx;

    // 현재 디렉토리의 모든 파일 목록 가져옴
    if ((count = scandir(root_path, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", root_path, strerror(errno));
        return;
    }

    // 파일 목록을 순회하면서 읽어온다.
    for (idx = 0; idx < count; ++idx) {
        char *temp_d_name = namelist[idx]->d_name;

        // 만약 현재 디렉토리나 부모 디렉토리를 가르키면 탐색을 종료한다.
        if (!strcmp(temp_d_name, ".") || !strcmp(temp_d_name, "..")) {
            continue;
        }

        // 찾아야하는 루트가 있으면 다 합치고 없으면 하나만 합침
        if (search_path[0] != '\0')
            sprintf(temp_str, "%s/%s/%s", root_path, search_path, temp_d_name);
        else
            sprintf(temp_str, "%s/%s", root_path, temp_d_name);

        if (get_same_file(
                filename,
                found_files[0],
                temp_str,
                &found_files[*file_index])
                ) {
            if (search_path[0] != '\0') sprintf(found_files[*file_index].name, "%s/%s", search_path, temp_d_name);
            else {
                sprintf(found_files[*file_index].name, "%s", temp_d_name);
            }
            (*file_index)++;
        }

        if (is_dir(temp_str)) {
            // 만약 찾은 부분이 디렉토리면 재귀적으로 탐색한다.
            sprintf(temp_str, "%s/%s", search_path, temp_d_name);
            find_files(filename, root_path, temp_str, found_files, file_index);
        }

        memset(temp_str, 0, sizeof temp_str);
    }

    for (idx = 0; idx < count; idx++) {
        free(namelist[idx]);
    }

    free(namelist);
}

void print_files_info(my_file *found_files, int num, int *stopflag) {
    // 찾은 파일들 출력
    print_file_col();
    for (int i = 0; i < num; ++i) {
        print_file_info(i, found_files[i]);
    }
    if (num == 1) {
        // 아무것도 못 찾았으면 stopflag=1로 하여 이후에 find prompt 실행을 막음
        printf("(None)\n");
        (*stopflag)++;
    }
}

int parse_option_value(char keyword[][STR_BUFFER_SIZE]) {
    // keyword[1]부터 option 값이 저장되어 있음
    int i = 1, res = 0;
    char *s = keyword[i++];
    while (s[0] != '\0') {
        // q = 1
        if (!strcmp(s, "q")) res |= 1;
            // s = 1 << 1
        else if (!strcmp(s, "s")) res |= 1 << 1;
            // s = 1 << 2
        else if (!strcmp(s, "i")) res |= 1 << 2;
            // s = 1 << 3
        else if (!strcmp(s, "r")) res |= 1 << 3;
        else if (!strcmp(s, "l")) res |= 1 << 4;
        s = keyword[i++];
    }
    return res;
}

void find_prompt(my_file *found_files) {
    char inputBuffer[STR_BUFFER_SIZE] = {0}, keywords[10][STR_BUFFER_SIZE] = {0};
    int option = 0, index = 0;
    printf(">> ");
    fgets(inputBuffer, STR_BUFFER_SIZE, stdin);

    // keyword parse!
    slicing_str(inputBuffer, keywords);

    // keyword[0]에 index 값 저장되어 있음
    index = (int) strtol(keywords[0], NULL, 10);
    // 2진수로 option 값 저장
    option = parse_option_value(keywords);

    diff(found_files[0], found_files[index], option);
}

void ssu_find(char *filename, char *path) {
    // find 와 관련된 모든 로직

    char full_path[STR_BUFFER_SIZE] = {}, search_path[STR_BUFFER_SIZE] = {};
    my_file found_files[FILE_BUFFER_SIZE];
    int file_index = 1, stopflag = 0;

    // 선택된 파일 전체 경로 저장
    sprintf(full_path, "%s/%s", path, filename);

    found_files[0].fd = fopen(full_path, "r");

    // 만약 file descriptor가 null 이라면 에러처리 후 prompt로
    if (found_files[0].fd == 0) {
        fprintf(stderr, "error for %s", filename);
        fflush(stderr);
        printf("\n");
        return;
    }

    // 초기 파일 처리
    stat(full_path, &found_files[0].st);
    realpath(full_path, found_files[0].path);
    strcpy(found_files[0].name, filename);

    find_files(filename, path, search_path, found_files, &file_index);

    print_files_info(found_files, file_index, &stopflag);

    if (!stopflag) {
        find_prompt(found_files);
    }

    close_files(found_files, file_index);
}
