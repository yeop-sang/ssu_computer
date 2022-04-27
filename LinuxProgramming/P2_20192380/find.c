#include "find.h"

int get_list_in_set(myFileListSet **result_ptr,
                    myFileListSet **before_ptr, int set_inex) {
    for (int i = 0; i < set_inex; ++i) {
        // 범위를 벗어난 경우 에러처리
        if (*result_ptr == NULL) {
            fprintf(stderr, "Range Error with set index\n");
            return -1;
        }
        *before_ptr = *result_ptr;
        *result_ptr = (*result_ptr)->next;
    }
    return 0;
}

int get_info_in_list(myFileList **result_ptr, myFileList **before_ptr, int list_index) {
    for (int i = 0; i < list_index; ++i) {
        if (*result_ptr == NULL) {
            fprintf(stderr, "Range Error with list index\n");
            return -1;
        }
        *before_ptr = *result_ptr;
        *result_ptr = (*result_ptr)->next;
    }
    return 0;
}

int get_info_in_set(myFileListSet **cur, myFileList **result_ptr, myFileList **before_ptr,
                    int set_index, int list_index) {
    myFileListSet *list_ptr = *cur, *tmp = NULL;
    if (get_list_in_set(&list_ptr, &tmp, set_index) < 0) {
        // 범위를 벗어난 경우 에러 처리
        return -1;
    }

    *cur = list_ptr;

    *result_ptr = list_ptr->list;
    if (get_info_in_list(result_ptr, before_ptr, list_index) < 0) {
        return -1;
    }

    return 0;
}

int option_d(myFileListSet *head, int set_index, int list_index) {
    // 해당 위치를 탐색함
    myFileList *info_ptr = NULL, *before_ptr = NULL;
    if (get_info_in_set(&head, &info_ptr, &before_ptr, set_index, list_index) < 0)
        // 범위를 벗어난 경우 오류 처리
        return -1;

    if (before_ptr == NULL) {
        // 삭제 해야하는 곳이 처음이면 리스트의 처음 원소로 옮겨주고 삭제

        head->list = info_ptr->next;
        free_myFileLinkedList(info_ptr);
    } else {
        // 아니라면 중간의 데이터 삭제 후 삽입
#ifndef DEBUG
        remove(info_ptr->info->path);
#endif
        before_ptr->next = info_ptr->next;
        free_myFileLinkedList(info_ptr);
    }

    return 0;
}

int option_i(myFileListSet *head, int set_index) {
    myFileListSet *set_ptr = head, *before_set_ptr = NULL;
    if (get_list_in_set(&set_ptr, &before_set_ptr, set_index) < 0) {
        return -1;
    }

    myFileList *list_ptr = set_ptr->list, *before_ptr = NULL;

    char input[BUFFER_SIZE];

    // 반복을 하면서 지울지 물어봄!
    while (list_ptr != NULL) {
        printf("Delete \"%s\"? [y/n]", list_ptr->info->path);
        fgets(input, BUFFER_SIZE, stdin);
        // 대문자가 들어와도 소문자로 변환
        input[0] = tolower(input[0]);
        if (input[0] == 'y') {
            // y 가 들어오면 제거
            if (before_ptr == NULL) {
                // 처음 원소를 제거하는 것을 처리함
                set_ptr->list = list_ptr->next;
#ifndef DEBUG
                remove(list_ptr->info->path);
#endif
                free_myFileLinkedList(list_ptr);
                list_ptr = set_ptr->list;
            } else {
                // 중간 원소를 제거함!
                before_ptr->next = list_ptr->next;
#ifndef DEBUG
                remove(list_ptr->info->path);
#endif
                free_myFileLinkedList(list_ptr);
                list_ptr = before_ptr->next;
            }
            continue;
        }

        // 다음칸으로 이동
        before_ptr = list_ptr;
        list_ptr = list_ptr->next;
    }

    return 0;
}

// 전체 경로에서 파일명만 추출하는 함수
char *extract_name(char *string) {
    char *filename = strrchr(string, '/');
    return filename ? filename + 1 : string;
}

void move_trash(char *old_path) {
    char new_path[PATH_MAX];
    sprintf(new_path, "%s/%s", TRASH_CAN_PATH, extract_name(old_path));
    if (rename(old_path, new_path) < 0) {
        fprintf(stderr, "Error moving trash can from %s to %s(%d)\n", old_path, new_path, errno);
        remove(old_path);
    }
}

myFileList *get_current_file(myFileList *head) {
    myFileList *current_file = head;
#ifdef DEBUG
    print_myFileList(head);
#endif
    // 순회하면서 가장 작은 애를 저장함
    while (head != NULL) {
        // 지금 node가 최신이면
#ifdef DEBUG
        print_myFileList(head);
        printf("current (mtime : %s), ptr (mtime: %s)\n",
               get_file_mtime(current_file),
               get_file_mtime(head)
        );
#endif
        if (strcmp(get_file_mtime(current_file), get_file_mtime(head)) < 0) {
            // 그 정보를 저장한다.
            current_file = head;
        }
        head = head->next;
    }

    return current_file;
}

int option_ft(myFileListSet **head, int set_index, int trash_flag) {
    myFileListSet *result = *head, *before_ptr = NULL;
    char *current_mtime;
    if (get_list_in_set(&result, &before_ptr, set_index) < 0) {
        fprintf(stderr, "Error in option_f\n");
        return -1;
    }
#ifdef DEBUG
    print_myFileList(result->list);
#endif

    myFileList *list_ptr = result->list,
            *current_file = get_current_file(result->list);

#ifdef DEBUG
    print_myFileList(result->list);
#endif
    while (list_ptr != NULL) {
        if (list_ptr != current_file) {
            if (trash_flag) {
                // t 옵션의 경우
#ifndef DEBUG
                // 쓰레기 통으로 이동
                move_trash(list_ptr->info->path);
#endif
            } else {
                // f 옵션의 경우
#ifndef DEBUG
                // 그냥 제거
                remove(list_ptr->info->path);
#endif
            }
        }
        list_ptr = list_ptr->next;
    }

    if (trash_flag)
        printf("All files in #%d have moved to Trash except \"%s\" (%s)\n",
               set_index + 1,
               current_file->info->path,
               get_file_mtime(current_file)
        );
    else {
        printf("Left file in #%d : %s (%s)\n",
               set_index,
               current_file->info->path,
               get_file_mtime(current_file)
        );
    }

    // 출력 이후 해당 set 제거
    if (before_ptr == NULL) {
        // index가 0이면 head 주소 변경!
        *head = result->next;
        free_myFileLinkedList(result->list);
        free(result);
    } else {
        // 아니라면 노드 중간에서 제거
        before_ptr->next = result->next;
        free_myFileLinkedList(result->list);
        free(result);
    }

    printf("\n");

    return 0;
}

void find_prompter(myFileListSet *head) {
    // 쓰레기통 생성

    if (mkdir(TRASH_CAN_PATH, 0755) < 0) {
        if (errno != 17) {
            fprintf(stderr, "Trash can(%s) cannot be created.(%d)\n", TRASH_CAN_PATH, errno);
            exit(1);
        }
    }

    char input[BUFFER_SIZE], *argv[MAX_ARGV];
    int argc;
    char *dump;
    int set_index, list_index;
    while (1) {

        // 남아 있는게 없으면 프롬프트 출력
        if (head == NULL) {
            break;
        }

        printf(">> ");
        fflush(stdout);

        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';

        argc = tokenization(input, " ", argv);

        // exit이면 프롬프트 출력
        if (!strcmp(argv[0], "exit")) {
            printf("Back to Prompt\n");
            break;
        }

        set_index = strtol(argv[0], &dump, 10) - 1;
        if (argc < 2) {
            fprintf(stderr, "Usage: [SET_INDEX] [OPTION]\n");
            continue;
        }

        // d 입력시
        if (argv[1][0] == 'd') {
            list_index = strtol(argv[2], NULL, 10) - 1;
            if (option_d(head, set_index, list_index) < 0) {
                // 에러처리!
                fprintf(stderr, "Error in option d\n");
                continue;
            }
        }

        // i 입력시
        if (argv[1][0] == 'i') {
            if (option_i(head, set_index) < 0) {
                // 에러 처리
                fprintf(stderr, "Error in option i\n");
                continue;
            }
        }

        // f 입력시
        if (argv[1][0] == 'f') {
            if (option_ft(&head, set_index, 0) < 0) {
                // 에러처리!
                fprintf(stderr, "Error in option f\n");
                continue;
            }
        }

        // t 입력시
        if (argv[1][0] == 't') {
            if (option_ft(&head, set_index, 1) < 0) {
                // 에러처리!
                fprintf(stderr, "Error in option f\n");
                continue;
            }
        }

        // 삭제 후 1개 짜리 제거하고 남아있는 리스트 보여줌
        head = remove_only_one_node(head);
        print_myFileListSet(head);
    }
}

char *parse_file_extension(char *file_name) {
    for (int i = strlen(file_name) - 1; i >= 0; --i) {
        if (file_name[i] == '.')
            return file_name + (i + 1);
    }
    return NULL;
}

int find_char_start_index(char *str) {
    int i;
    for (i = 0; i < strlen(str); ++i) {
        if (str[i] < '0' || str[i] > '9')
            return i;
    }
    return -1;
}

// kb, mb 등을 byte 단위로 통일
off_t decimalize_byte_string(char *byte_str) {
    // 0이 들어오면 0만 반환
    if (strlen(byte_str) == 1 && byte_str[0] == '0') {
        return 0;
    }

    // ~만 들어왔으면 무한으로 넣어준다.
    if (strlen(byte_str) == 1 && byte_str[0] == '~') {
        return -1;
    }

    double byte;
    int char_start_point = find_char_start_index(byte_str);
    char *unit = byte_str + char_start_point;

    byte = strtof(byte_str, NULL);

    // 들어온 값이 숫자가 없으면서 오류가 나면 오류
    if (char_start_point == 0 && char_start_point == -1) {
        return -2;
    }

    // 단위를 소문자로 바꿔준다.
    for (int i = 0; i < strlen(unit); ++i) {
        unit[i] = tolower(unit[i]);
    }

    // unit 에 따라서 값을 곱해준다.
    if(char_start_point != -1) {
        if (!strcmp(unit, "kb")) {
            byte *= 1000;
        } else if (!strcmp(unit, "mb")) {
            byte *= 1000 * 1000;
        } else if (!strcmp(unit, "gb")) {
            byte *= 1000 * 1000 * 1000;
        } else {
            // kb, mb, gb 이외의 문자가 들어오면 오류!
            return -2;
        }
    }

    return (off_t) byte;
}

void find_command(char *file_extension, char *min_size_str, char *max_size_str, char *target_dir, char *hash_type) {
    off_t min_size, max_size;
    void *file_hasher;
    int digest_length;

#ifdef DEBUG
    printf("%s\n", target_dir);
#endif

    // 탐색 directory로 이동
    target_dir = realpath(target_dir, NULL);
    if (chdir(target_dir) < 0) {
        fprintf(stderr, "%s is not allowed to access, code : %d\n", target_dir, errno);
        exit(1);
    }

    // 바이트 파싱
    min_size = decimalize_byte_string(min_size_str);
    max_size = decimalize_byte_string(max_size_str);
    if (min_size < 0) {
        if (min_size == -1) {
            min_size = 0;
        } else {
            fprintf(stderr, "MIN_SIZE should be size[kb | mb | gb]");
            exit(1);
        }
    }
    if (max_size < 0) {
        // max_size 가 -1 이면 임의의 최대 값으로 바꾼다.
        if (max_size == -1)
            max_size = FILE_MAX_SIZE;
        else {
            fprintf(stderr, "MAX_SIZE should be size[kb | mb | gb]");
            exit(1);
        }
    }

    // 해싱 함수 선택
    if (!strcmp(hash_type, "md5")) {
        file_hasher = &hash_file_by_md5;
        digest_length = MD5_DIGEST_LENGTH;
    } else if (!strcmp(hash_type, "sha1")) {
        file_hasher = &hash_file_by_sha1;
        digest_length = SHA_DIGEST_LENGTH;
    } else {
        fprintf(stderr, "HASH_TYPE should be md5 | sha1");
        exit(1);
    }

    // 파일 확장자 읽어옴
    if (file_extension[0] != '*') {
        fprintf(stderr, "FILE_EXTENSION should be *[.<extension>]");
        exit(1);
    }

    if (strlen(file_extension) != 1) {
        file_extension = parse_file_extension(file_extension);
        if (file_extension == NULL) {
            fprintf(stderr, "FILE_EXTENSION should be *[.<extension>]");
            exit(1);
        }
    } else {
        file_extension = NULL;
    }

    find_dup(file_extension, min_size, max_size, target_dir, file_hasher, digest_length);
}

void find_dup(char *file_extension, off_t min_size, off_t max_size, char *target_dir,
              void (*file_hasher)(char *result, FILE *file_ptr), int digest_length) {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    // 파일 탐색
    char temp_hash_digest[digest_length * 2 + 1], temp_file_path[PATH_MAX], temp_work_directory[PATH_MAX];
    char *temp_file_extension;
    myDirQueue *dir_queue_head = create_myDirQueue_node(target_dir, 0);
    myFileListSet *file_list_list_head = NULL;
    int depth = 0;
    struct stat stat_buffer;

    while (is_myDirQueue_empty(dir_queue_head)) {
        pop_front_myDirQueue(&dir_queue_head, temp_work_directory, &depth);

        // /proc /sys /run directory는 제외한다.
        if (!strcmp(temp_work_directory, "/proc") || !strcmp(temp_work_directory, "/run") ||
            !strcmp(temp_work_directory, "/sys"))
            continue;

        // 현재 directory의 내용을 불러온다.
        int file_count;
        struct dirent **directory_entries;
        if ((file_count = scandir(temp_work_directory, &directory_entries, NULL, alphasort)) == -1) {
            fprintf(stderr, "There is a problem scanning %s!(%d)\n", temp_work_directory, errno);
            continue;
        }
#ifdef DEBUG
        print_myFileListSet(file_list_list_head);
        printf("search dir : %s(count: %d)\n", temp_work_directory, file_count - 2);
#endif

        for (int i = 0; i < file_count; ++i) {
            // 현재 directory 나 상위 directory 를 처리하면 loop가 일어나므로 처리하지 않는다.
            if (!strcmp(directory_entries[i]->d_name, ".") || !strcmp(directory_entries[i]->d_name, ".."))
                continue;

            // 확장자가 같은지 확인

            sprintf(temp_file_path, "%s/%s", temp_work_directory, directory_entries[i]->d_name);

            if (stat(temp_file_path, &stat_buffer) == -1) {
                fprintf(stderr, "There is a problem opening %s!(%d)\n", temp_file_path, errno);
                continue;
            }

#ifdef DEBUG
            printf("current file : %s(%lld)\nfile type : ", temp_file_path, stat_buffer.st_size);
            S_ISDIR(stat_buffer.st_mode) ? puts("dir\n") :
            S_ISREG(stat_buffer.st_mode) ? puts("reg\n") :
            puts("something else\n");
#endif

            // 해당 파일이 directory 면 push 한다.
            if (S_ISDIR(stat_buffer.st_mode)) {
                enqueue_myDirQueue(
                        &dir_queue_head,
                        create_myDirQueue_node(temp_file_path, depth + 1)
                );
            }
                // 해당 파일이 일반 파일이면 list에 삽입함
            else if (S_ISREG(stat_buffer.st_mode)) {
                // 파일 크기가 0이면 넘어간다.
                if (!stat_buffer.st_size)
                    continue;

                // file의 확장자가 같은지 처리
                temp_file_extension = parse_file_extension(temp_file_path);
                if (file_extension != NULL) {
                    if (temp_file_extension == NULL)
                        continue;
                    if (strcmp(temp_file_extension, file_extension) != 0)
                        continue;
                }

                // 파일의 크기가 제한 범위 안에 있을때만 삽입함!
                if (stat_buffer.st_size >= min_size && stat_buffer.st_size <= max_size) {
                    FILE *file_ptr = fopen(temp_file_path, "r");
                    if (file_ptr == NULL) {
                        fprintf(stderr, "There is a problem opening %s!(%0x)\n", temp_file_path, errno);
                        continue;
                    }
                    file_hasher(temp_hash_digest, file_ptr);
                    file_list_list_head = insert_myFileListLinkedList(
                            file_list_list_head,
                            create_file_linked_node(
                                    create_fileInfo(
                                            temp_file_path,
                                            depth,
                                            stat_buffer.st_mtime,
                                            stat_buffer.st_atime
                                    )
                            ),
                            stat_buffer.st_size,
                            temp_hash_digest
                    );
                    fclose(file_ptr);
                }
            }
        }

#ifdef DEBUG
        print_myFileListSet(file_list_list_head);
#endif
    }

    // 원소가 한 개만 있는 것을 제거한다.
    file_list_list_head = remove_only_one_node(file_list_list_head);
    gettimeofday(&end_time, NULL);

    end_time.tv_sec -= start_time.tv_sec;
    if (end_time.tv_usec < start_time.tv_usec) {
        end_time.tv_sec--;
        end_time.tv_sec += (long) 1e6;
    }
    end_time.tv_usec -= start_time.tv_usec;

    print_myFileListSet(file_list_list_head);
    printf("Runtime: %ld:%06d(sec:usec)\n", end_time.tv_sec, end_time.tv_usec);

    find_prompter(file_list_list_head);

    free(file_list_list_head);
    free(target_dir);
}