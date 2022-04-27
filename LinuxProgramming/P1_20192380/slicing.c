// 문자열과 관련된 함수를 모아둠
#include "slicing.h"

void slicing_str(const char *in_str, char keyword_buffer[][STR_BUFFER_SIZE]) {
    int keyword_cnt = 0, temp_cnt = 0;
    char temp_buffer[STR_BUFFER_SIZE] = {};
    for (int i = 0; i < strlen(in_str); ++i) {
        if (in_str[i] == ' ' || in_str[i] == '\n') {
            strcpy(keyword_buffer[keyword_cnt++], temp_buffer);
            memset(temp_buffer, 0, STR_BUFFER_SIZE);
            temp_cnt = 0;
        } else {
            temp_buffer[temp_cnt++] = in_str[i];
        }
        if (in_str[i] == '\n') {
            break;
        }
    }
}

void path_to_fname(const char *path, char *output) {
    int output_cnt = 0;
    for (int i = 0; i < strlen(path); ++i) {
        if (path[i] == '/') {
            memset(output, 0, output_cnt);
            output_cnt = 0;
        } else output[output_cnt++] = path[i];
    }
    output[output_cnt] = '\0';
}

