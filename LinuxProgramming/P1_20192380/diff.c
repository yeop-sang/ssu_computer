
#include "diff.h"

#define MAX(a, b) ((a)>(b)? (a):(b))

// str[0]은 무조건 '\0',

void print_str(char str[][STR_BUFFER_SIZE], int i) {
    // 문자열 마지막에 개행이 포함되어 있으면 그대로 출력
    if (str[i][strlen(str[i]) - 1] == '\n')
        printf("%s", str[i]);
        // 아니면 개행을 포함해 출력 후 메세지 전송
    else {
        printf("%s\n", str[i]);
        printf("\\No new line at end of file\n");
    }
}

void
print_append(char str1[][STR_BUFFER_SIZE], char str2[][STR_BUFFER_SIZE], int o1, int o2, int n1, int n2, int option) {
    printf("%da%d", o1, n1);
    if (n1 == n2) printf("\n");
    else printf(",%d\n", n2);
    // l 옵션이 꺼져있을때만 문자열 출력
    if (!(option & 1 << 4))
        for (int i = n1; i <= n2; ++i) {
            printf("> ");
            print_str(str2, i);
        }
}

void
print_delete(char str1[][STR_BUFFER_SIZE], char str2[][STR_BUFFER_SIZE], int o1, int o2, int n1, int n2, int option) {
    if (o1 == o2) printf("%d", o1);
    else printf("%d,%d", o1, o2);
    printf("d");
    printf("%d\n", n1);
    // l 옵션이 꺼져있을때만 문자열 출력
    if (!(option & 1 << 4))
        for (int i = o1; i <= o2; ++i) {
            printf("< ");
            print_str(str1, i);
        }
}

void
print_change(char str1[][STR_BUFFER_SIZE], char str2[][STR_BUFFER_SIZE], int o1, int o2, int n1, int n2, int option) {
    // 위 아래 줄이 둘 다 같이 개행이면 변경되지 않은 것으로 본다.
    // 개행은 다른 것으로 보기로 간주했기 때문
    if (str1[o1][0] == '\n' && str2[n1][0] == '\n')
        o1++, n1++;
    if (str1[o2][0] == '\n' && str2[n2][0] == '\n')
        o2--, n2--;

    // 개행 처리로 이상해진 것은 출력하지 않음
    if (o1 > o2 || n1 > n2)
        return;

    // 변경된 줄이 한 줄이면 하나만 출력한다.
    if (o1 == o2) printf("%d", o1);
    else printf("%d,%d", o1, o2);
    printf("c");
    if (n1 == n2) printf("%d", n1);
    else printf("%d,%d", n1, n2);
    printf("\n");

    // l 옵션이 꺼져있을때만 문자열 출력
    if (!(option & 1 << 4)) {
        for (int i = o1; i <= o2; ++i) {
            printf("< ");
            print_str(str1, i);
        }
        printf("---\n");
        for (int i = n1; i <= n2; ++i) {
            printf("> ");
            print_str(str2, i);
        }
    }
}

int get_strings(FILE *fd, char buffer[][STR_BUFFER_SIZE]) {
    // 파일 전체에서 문자열 읽어옴. 0번 라인은 무조건 0
    // 최종적으로 문자열 갯수 + 1 출력
    int i = 1;
    while (feof(fd) == 0) {
        fgets(buffer[i], STR_BUFFER_SIZE, fd);
        i++;
    }

    return i - 1;
}

void ff_until_1(const int *ls, int *s, int limit) {
    // 1만날때 까지 앞으로 돌림
    for (; *s <= limit; (*s)++)
        if (ls[*s])
            break;
    // 마지막일때는 1을 더하여 뒤에서 -1할때 문제 없도록 함
//    if (limit == (*s))(*s)++;
}

int is_sandwiched_blank(char str[][STR_BUFFER_SIZE], int i, int limit) {
    int flag = 0;
    if (i > 1 && !str[i - 1][0])
        if (i < limit && !str[i + 1][0])
            flag = 1;
    return flag;
}

void remove_sandwched_blank(int cmp[], char str[][STR_BUFFER_SIZE], int i, int limit) {
    // 중간에 혼자 낀 공백이 1이면 제거함
    if (cmp[i] == 1)
        if (i > 0 && cmp[i - 1] == 0)
            if (i <= limit && cmp[i + 1] == 0)
                if (str[i][0] == '\n')
                    cmp[i] = 0;
}

void
print_file_diff(char str1[][STR_BUFFER_SIZE], char str2[][STR_BUFFER_SIZE], int cmp1[], int cmp2[], int len1,
                int len2, int option) {
    int i = 0, j = 0;
    int o1 = 0, o2 = 0, n1 = 0, n2 = 0;
    while (1) {
        // 둘 다 범위를 벗어나면 종료
        if (i > len1 && j > len2)
            break;

        // 원본은 종료되었지만 비교본은 종료되지 않은 경우 append
        if (i > len1) {
            print_append(str1, str2, len1, o2, j, len2, option);
            break;
        }

        // 비교본은 종료되었지만 원본은 종료되지 않은 경우 delete
        if (j > len2) {
            print_delete(str1, str2, i, len1, len2, n2, option);
            break;
        }

        int origin = cmp1[i], compare = cmp2[j];

        // 둘 다 공통 문자열이면 같음
        if (origin && compare) {
            i++;
            j++;
            continue;
        }

        // 원본만 공통 문자열이면 append
        if (origin) {
            // append는 같기 이전에 처리 됨
            o1 = i - 1;
            n1 = j;
            // 공통 문자열이 나올때까지 감음
            ff_until_1(cmp2, &j, len2);
            n2 = j - 1;

            print_append(str1, str2, o1, o2, n1, n2, option);
            continue;
        }

        // 비교본만 공통 문자열이면 delete
        if (compare) {
            o1 = i;
            // 공통 문자열이 나올때까지 감음
            ff_until_1(cmp1, &i, len1);
            o2 = i;
            n1 = j - 1;

            print_delete(str1, str2, o1, o2, n1, n2, option);
            continue;
        }

            // 모두가 0인 경우 change
        else {
            // 초기 경우 설정
            if (i == 0 && j == 0) {
                // 둘 다 다음 행으로 넘겨서 비교하자
                int temp_origin, temp_compare;
                temp_origin = cmp1[i + 1], temp_compare = cmp2[j + 1];

                // 만약 둘 다 공통 문자열이라면 둘 다 증가
                if (temp_compare && temp_origin) {
                    i++;
                    j++;
                    continue;
                } else if (temp_compare) {
                    // 복사본만 공통 문자열이라면 delete된 것
                    i++; // 항상 1행부터 문자열이 존재함!

                    o1 = i;
                    // 공통 문자열이 나올때까지 감음
                    ff_until_1(cmp1, &i, len1);
                    o2 = i - 1;
                    n1 = j;

                    print_delete(str1, str2, o1, o2, n1, n2, option);

                    // 정상적으로 진행하기 위해 j 를 증가
                    j++;
                    continue;
                } else if (temp_origin) {
                    // 원본만 공통 문자열이라면 append된 것
                    j++; // 항상 1행부터 문자열이 존재함!

                    o1 = i;
                    n1 = j;
                    // 공통 문자열이 나올때까지 감음
                    ff_until_1(cmp2, &j, len2);
                    n2 = j - 1;

                    print_append(str1, str2, o1, o2, n1, n2, option);

                    // 정상적으로 진행하기 위해 i 를 증가
                    i++;
                    continue;
                }
                // 둘 다 아니라면 change
                // 아래에서 판별함!
            }

            // change 판별
            // 원본 행과 비교 행이 같을때까지 둘 행번호 다 증가
            o1 = i;
            ff_until_1(cmp1, &i, len1);
            o2 = i - 1;

            n1 = j;
            ff_until_1(cmp2, &j, len2);
            n2 = j - 1;
            print_change(str1, str2, o1, o2, n1, n2, option);

            continue;
        }
    }
}

void diff_file(my_file f1, my_file f2, int option) {
    // 문자열 목록을 저장해두는 변수
    char str1[STR_BUFFER_SIZE][STR_BUFFER_SIZE] = {0},
            str2[STR_BUFFER_SIZE][STR_BUFFER_SIZE] = {0};
    // 저장된 문자열의 총 갯수를 저장하는 변수
    int len1, len2;
    // 파일의 맨 마지막 줄이 개행인지 아닌지 저장하는 변수
    int end_line_flag1, end_line_flag2;
    len1 = get_strings(fopen(f1.path, "r"), str1);
    len2 = get_strings(fopen(f2.path, "r"), str2);

    // lcs 를 위한 table 설정
    int lcs[len1 + 1][len2 + 1];
    memset(lcs, 0, sizeof(int) * (len1 + 1) * (len2 + 1));

    // 비교 상태를 저장하기 위한 값 설정
    int cmp1[len1 + 1], cmp2[len2 + 1];
    memset(cmp1, 0, sizeof(int) * (len1 + 1));
    memset(cmp2, 0, sizeof(int) * (len2 + 1));

    // lcs 경로를 위한 테이블 값 설정
    // 대소문자 비교를 끄기 위한 함수 설정
    int (*my_strcmp)(const char *__s1, const char *__s2_) = option & (1 << 2) ? strcasecmp : strcmp;
    int blank_flag1 = 0, blank_flag2 = 0;
    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            if (!my_strcmp(str1[i], str2[j])) {
                // 같으면 대각선으로 진행한다.
                lcs[i][j] = lcs[i - 1][j - 1] + 1;
            }
                // 아니면 다른 줄에서 값 빌려옴
            else lcs[i][j] = MAX(lcs[i - 1][j], lcs[i][j - 1]);
        }
    }
#ifdef DEBUG
    for (int i = 0; i <= len1; ++i) {
        for (int j = 0; j <= len2; ++j) {
            printf("%3d ", lcs[i][j]);
        }
        printf("\n");
    }
#endif
    int i = len1, j;
    for (j = len2; j > 0;) {
        if (i < 0) break;
#ifdef DEBUG
        printf("%d(%s) %d(%s) :", i, str1[i], j, str2[j]);
        printf("%d %d %d\n", lcs[i][j], lcs[i - 1][j], lcs[i][j - 1]);
#endif
        if (lcs[i][j] == lcs[i - 1][j]) {
            // 현재 개행이면 저장
            if (str1[i][0] == '\n')
                blank_flag1 = i;
            cmp1[i--] = 0;
        } else if (lcs[i][j] == lcs[i][j - 1]) {
            // 현재 개행이면 이동 전에 저장
            if (str2[j][0] == '\n')
                blank_flag2 = j;
            cmp2[j--] = 0;
        } else if (lcs[i - 1][j] == lcs[i][j - 1]) {
            // 같은 두 문자열이 개행일 때
            if (str1[i][0] == '\n') {
#ifdef DEBUG
                printf("%d(%d,%s) %d(%d,%s)\n", blank_flag1, cmp1[blank_flag1 + 1], str1[blank_flag1 + 1], blank_flag2,
                       cmp2[blank_flag2 + 1], str2[blank_flag2 + 1]);
                printf("%d(%d,%s) %d(%d,%s)\n", blank_flag1, cmp1[blank_flag1 + 1], str1[blank_flag1 + 1], j,
                       cmp2[j + 1], str2[j + 1]);
                printf("%d(%d,%s) %d(%d,%s)\n", i, cmp1[i + 1], str1[i + 1], blank_flag2, cmp2[blank_flag2 + 1],
                       str2[blank_flag2 + 1]);
#endif
                if (blank_flag1 < len1 && cmp1[blank_flag1 + 1] && blank_flag2 < len2 && cmp2[blank_flag2 + 1]) {
                    // 공백 처리 이전에 같다고 처리한 문자열이 있으면 해당 공백도 같은 문자열이라고 처리
                    cmp1[i--] = cmp2[j--] = 1;
                } else if (blank_flag1 < len1 && cmp1[blank_flag1 + 1] && j < len2 && cmp2[j + 1]) {
                    // 원본 문자열에서 공백처리 되기 전 문자열과 비교 중인 직전 문자열이 같으면 해당 공백도 같은 문자열이라고 처리
                    cmp1[i--] = cmp2[j--] = 1;
                } else if (i < len1 && cmp1[i + 1] && blank_flag2 < len2 && cmp2[blank_flag2 + 1]) {
                    // 원본 직전 문자열과 비교 중인 문자열에서 공백처리 되기 전 문자열이 같으면 해당 공백도 같은 문자열이라고 처리
                    cmp1[i--] = cmp2[j--] = 1;
                } else if (str1[i + 1][0] == '\n' && str2[j + 1][0] == '\n') {
                    // 연속된 개행일때 그 이전 개행들을 같다고 판별
                    cmp1[i + 1] = cmp2[j + 1] = 1;
                    // 그리고 현재 문자열은 같지 않다고 판별
                    cmp1[i--] = cmp2[j--] = 0;
                } else {
                    // 아니면 같지 않다고 판별
                    cmp1[i--] = cmp2[j--] = 0;
                }
                // 개행인 경우 그 상태를 저장함
                blank_flag1 = i + 1, blank_flag2 = j + 1;
            } else {
                // 개행이 아닌 경우엔 그냥 진행
                cmp1[i--] = cmp2[j--] = 1;
            }
        }
    }

#ifdef DEBUG
    for (int k = 0; k <= len1; ++k) {
        printf("%d ", cmp1[k]);
    }
    printf("\n");
    for (int k = 0; k <= len2; ++k) {
        printf("%d ", cmp2[k]);
    }
    printf("\n");
#endif

    //option q 와 s 처리
    if (!(option & 1) && !(option & 1 << 1)) {
        // 둘 다 참이 아닐때만 해당 내용을 출력함
        print_file_diff(str1, str2, cmp1, cmp2, len1, len2, option);
    } else {
        // q가 참일때
        if (option & 1) {
            int sum = 0;
            for (int k = 1; k < len1 + 1; ++k) {
                sum += cmp1[k];
            }
            for (int k = 1; k < len2 + 1; ++k) {
                sum += cmp2[k];
            }
            if (sum != (len1 + len2))
                printf("Files %s and %s differ\n", f1.name, f2.name);
        }
        // s가 참일때
        if (option & 1 << 1) {
            int sum = 0;
            for (int k = 1; k < len1 + 1; ++k) {
                sum += cmp1[k];
            }
            for (int k = 1; k < len2 + 1; ++k) {
                sum += cmp2[k];
            }
            if (sum != (len1 + len2))
                printf("Files %s and %s are identical\n", f1.name, f2.name);
        }

    }

}

void diff_dir(my_file f1, my_file f2, int option) {
    struct dirent **namelist1, **namelist2;
    int cnt1, cnt2;
    // f1에서 찾은 파일 이름과 f2에서 찾은 파일 이름을 탐색함.
    char f1_file_name[STR_BUFFER_SIZE], f2_file_name[STR_BUFFER_SIZE];

    // 현재 디렉토리의 모든 파일 목록 가져옴
    if ((cnt1 = scandir(f1.path, &namelist1, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error\n", f2.path);
        return;
    }

    if ((cnt2 = scandir(f2.path, &namelist2, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error\n", f2.path);
        return;
    }

    // lcs 를 위한 table 설정
    int lcs[cnt1 + 1][cnt2 + 1];
    memset(lcs, 0, sizeof(int) * (cnt1 + 1) * (cnt2 + 1));

    // 비교 상태를 저장하기 위한 값 설정
    int cmp1[cnt1 + 1], cmp2[cnt2 + 1];
    memset(cmp1, 0, sizeof(int) * (cnt1 + 1));
    memset(cmp2, 0, sizeof(int) * (cnt2 + 1));

    // lcs 경로를 위한 테이블 값 설정
    int blank_flag1 = 0, blank_flag2 = 0;
    for (int i = 1; i < cnt1; ++i) {
        for (int j = 1; j < cnt2; ++j) {
            if (!strcmp(namelist1[i]->d_name, namelist2[i]->d_name)) {
                // 같으면 대각선으로 진행한다.
                lcs[i][j] = lcs[i - 1][j - 1] + 1;
            }
                // 아니면 다른 줄에서 값 빌려옴
            else lcs[i][j] = MAX(lcs[i - 1][j], lcs[i][j - 1]);
        }
    }

    int i = cnt1 - 1, j;
    for (j = cnt2 - 1; j > 0;) {
#ifdef DEBUG
        printf("%d() %d() :", i, j);
        printf("%d %d %d\n", lcs[i][j], lcs[i - 1][j], lcs[i][j - 1]);
#endif
        if (lcs[i][j] == lcs[i - 1][j]) {
            cmp1[i--] = 0;
        } else if (lcs[i][j] == lcs[i][j - 1]) {
            cmp2[j--] = 0;
        } else if (lcs[i - 1][j] == lcs[i][j - 1]) {
            cmp1[i--] = cmp2[j--] = 1;
        }
    }

#ifdef DEBUG
    for (int k = 0; k <= cnt1; ++k) {
        printf("%d ", cmp1[k]);
    }
    printf("\n");
    for (int k = 0; k <= cnt2; ++k) {
        printf("%d ", cmp2[k]);
    }
    printf("\n");
#endif

    i = 1, j = 1;
    while (1) {
        my_file temp1, temp2;
        // 두 이름 저장 배열 모두 끝났으면 종료
        if (i >= cnt1 && j >= cnt2)
            break;
        strcpy(temp1.name, namelist1[i]->d_name);
        strcpy(temp2.name, namelist2[j]->d_name);

        // 하나만 끝났으면 다른 친구 끝까지 일치하지 않는다고 출력
        if (i >= cnt1) {
            for (int k = j; k < cnt2; k++) {
                printf("Only in %s: %s\n", f2.name, namelist2[k]->d_name);
            }
            break;
        }
        if (j >= cnt2) {
            for (int k = i; k < cnt1; k++) {
                printf("Only in %s: %s\n", f1.name, namelist1[k]->d_name);
            }
            break;
        }

        // 만약 현재 디렉토리나 부모 디렉토리를 가르키면 1에 대한 탐색을 종료한다.
        if (!strcmp(temp1.name, ".") || !strcmp(temp1.name, "..")) {
            i++;
            continue;
        }

        if (!strcmp(temp2.name, ".") || !strcmp(temp2.name, "..")) {
            j++;
            continue;
        }

        int origin = cmp1[i], compare = cmp2[j];

        // 둘 다 공통 문자열이면 file diff 시작
        if (origin && compare) {
            sprintf(temp1.path, "%s/%s", f1.path, temp1.name);
            sprintf(temp2.path, "%s/%s", f2.path, temp2.name);
            diff_file(temp1, temp2, option);
            i++, j++;
        } else if (origin) {
            // 원본만 있으면 비교 대상에만 있음
            printf("Only in %s: %s\n", f2.name, namelist2[j++]->d_name);

        } else if (compare) {
            // 비교 대상에 없으면 원본에만 있음
            printf("Only in %s: %s\n", f1.name, namelist1[i++]->d_name);
        } else {
            // 둘 다 다르면 1이 뜰 때까지 돌림
            // 원본만 있는 경우
            while (!cmp1[i++])
                printf("Only in %s: %s\n", f1.name, namelist1[i]->d_name);
            // 비교 대상에만 있는 경우
            while (!cmp2[j++])
                printf("Only in %s: %s\n", f2.name, namelist2[j]->d_name);
        }
    }
}


void diff(my_file f1, my_file f2, int option) {
    if (is_dir(f1.path)) {
        diff_dir(f1, f2, option);
    } else {
        diff_file(f1, f2, option);
    }
}
