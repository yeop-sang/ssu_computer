#include "my_structs.h"

// 파일 시간 출력을 위한 함수
void get_time(time_t stime, char *result) {
    struct tm *tm;

    tm = localtime(&stime);
    strftime(result, TIME_LENGTH, "%Y-%m-%d %H:%M:%S", tm);
}

char decimal_to_char(int i) {
    return '0' + i % 10;
}

int get_byte_digit(ssize_t byte) {
    int cnt = 0;
    while (byte != 0) {
        byte /= 10;
        cnt++;
    }
    return cnt;
}

void get_byte_to_string(ssize_t byte, char *result) {
    int len = get_byte_digit(byte);
    int cnt = 0;
    // 3의 배수가 아니면 , 개수는 len / 3이다
    if (len % 3)
        len += len / 3;
        // 3의 배수면 , 개수는 len / 3 -1 이다
    else
        len += len / 3 - 1;
    result[len] = '\0';
    for (int i = 1; i <= len; i++) {
        if (cnt == 3) {
            cnt = 0;
            result[len - i] = ',';
            continue;
        }
        result[len - i] = decimal_to_char(byte % 10);
        cnt++;
        byte /= 10;
    }
}

// directory 정보를 저장할 Queue node 생성
myDirQueue *create_myDirQueue_node(char *dir_path, int depth) {
    myDirQueue *ptr = malloc(sizeof(myDirQueue));
    strcpy(ptr->dir_path, dir_path);
    ptr->depth = depth;
    ptr->next = NULL;
    return ptr;
}

// directory 정보를 queue 의 맨 뒤에 넣는 함수
void enqueue_myDirQueue(myDirQueue **head, myDirQueue *node) {
    if (*head == NULL) {
        *head = node;
    } else {
        myDirQueue *ptr = *head;
        while (ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = node;
        node->next = NULL;
    }
}

// directory 정보를 맨 앞에서 꺼내오는 함수
void pop_front_myDirQueue(myDirQueue **head, char *result, int *depth) {
    myDirQueue *next = (*head)->next;
    strcpy(result, (*head)->dir_path);
    *depth = (*head)->depth;
    free(*head);
    *head = next;
}

// queue 가 비어있는지 확인하는 함수
int is_myDirQueue_empty(myDirQueue *head) {
    return head != NULL;
}

// queue를 파괴하는 함수
// TODO: 사용하지 않을 시 함수 제거
void destroy_queue(myDirQueue *head) {
    myDirQueue *next_ptr = head->next;
    while (1) {
        free(head);
        head = next_ptr;
        if (head != NULL)
            break;
        next_ptr = next_ptr->next;
    }
}

// 파일 정보를 저장하는 linked_list의 노드 생성
myFileList *create_file_linked_node(fileInfo *info) {
    myFileList *node = malloc(sizeof(myFileList));
    node->info = info;
    node->next = NULL;
    return node;
}

// 파일 정보를 저장하는 linked list 메모리 제거
void free_myFileLinkedList(myFileList *node) {
    if (node->info != NULL)
        free(node->info);
    free(node);
}

// linked list 의 list 메모리 제거
void free_myFileListSet(myFileListSet *node) {
    myFileList* list_ptr = node->list, *next_list_ptr = NULL;
    while (list_ptr != NULL){
        next_list_ptr = list_ptr->next;
        free_myFileLinkedList(list_ptr);
        list_ptr = next_list_ptr;
    }
    free(node);
}

// 파일정보를 저장하는 list의 list를 생성하는 노드 생성
myFileListSet *create_list_linked_node(myFileList *node, size_t size, char *hash) {
    myFileListSet *list_node = malloc(sizeof(myFileListSet));
    list_node->list = node;
    list_node->next = NULL;
    list_node->size = size;
    strcpy(list_node->hash, hash);
    return list_node;
}

// file 정보를 myFileLinkedList에 삽입하는 작업
myFileList *insert_myFileLinkedList(myFileList *head, myFileList *insert_node) {
    if (head == NULL) {
        // 비어있다면 헤드 위치에 삽입한다.
        head = insert_node;
        return head;
    }

    myFileList *list_ptr = head, *before_list_ptr = NULL;
    /*
     * 순회하면서 삽입할 장소를 찾는다.
     *
     * 1. depth가 낮은 순으로 정리
     * 2. depth가 같다면 ascii 순으로 정리
     *
     * */
    while (1) {
        if (list_ptr == NULL) {
            // 비어있으면 전 포인터의 다음 원소로 지정
            before_list_ptr->next = insert_node;
            break;
        }

        if (list_ptr->info->depth < insert_node->info->depth) {
            // 현재 pointer 가 가리키는 depth 가 작으면 pointer를 옆으로 옮김
            before_list_ptr = list_ptr;
            list_ptr = list_ptr->next;
            continue;

        } else if (list_ptr->info->depth == insert_node->info->depth) {
            // depth 가 동일하면 ascii code를 비교함!
            if (strcmp(list_ptr->info->path, insert_node->info->path) < 0) {
                // 현재 ptr이 ascii code 가 낮다면 다음으로 이동!
                before_list_ptr = list_ptr;
                list_ptr = list_ptr->next;
                continue;

            } else {
                // 만약 크다면 삽입
                if (before_list_ptr == NULL) {
                    // 이동하지 않았다면 최상단에 insert 되어야함!
                    insert_node->next = list_ptr;
                    return insert_node;
                }

                before_list_ptr->next = insert_node;
                insert_node->next = list_ptr;
                break;
            }
        } else {
            if (before_list_ptr == NULL) {
                // 한 번도 이동하지 않았으면 최상단에 insert 되야함
                insert_node->next = list_ptr;
                return insert_node;
            }

            // 만약 list_ptr->node->depth가 크다면 전과 지금 위치 사이에 데이터를 삽입한다.
            before_list_ptr->next = insert_node;
            insert_node->next = list_ptr;
            break;
        }
    }
    return head;
}

// file list를 myFileListLinkedList에 삽입하는 작업
myFileListSet *
insert_myFileListLinkedList(myFileListSet *head, myFileList *node, size_t size, char *hash) {
    if (head == NULL) {
        // 비어있다면 새로운 file 정보 리스트를 생성하여 삽입한다.
        head = create_list_linked_node(node, size, hash);
        return head;
    }
    myFileListSet *list_ptr = head, *before_list_ptr = NULL;
    /*
     * 순회하면서 삽입할 장소를 찾는다.
     *
     * 1. 파일 크기가 낮은 순으로 정렬
     * 2. 해시 값이 같은 지 판단
     *
     * */
    while (1) {
        if (list_ptr == NULL) {
            // 현재 포인터가 비어있다면 새로운 file 정보 리스트를 생성하여 전 포인터에 삽입한다.
            before_list_ptr->next = create_list_linked_node(node, size, hash);
            break;
        }

        // 현재 pointer 가 가리키는 파일 크기가 작으면 pointer를 옆으로 옮김
        if (list_ptr->size < size) {
            before_list_ptr = list_ptr;
            list_ptr = list_ptr->next;
            continue;
        }

            // 현재 pointer 가 가리키는 파일 크기와 같으면 hash를 비교한다.
        else if (list_ptr->size == size) {
            // 같으면 해당 list에 데이터를 삽입
            if (!strcmp(list_ptr->hash, hash)) {
                list_ptr->list = insert_myFileLinkedList(list_ptr->list, node);
                // 그리고 정지시킨다.
                break;
            }

            // 같지 않으면 다음으로 이동
            before_list_ptr = list_ptr;
            list_ptr = list_ptr->next;
        } else {
            // 만약 파일 크기가 크다면 전과 지금 위치 사이에 데이터를 삽입한다.
            myFileListSet *insert_node
                    = create_list_linked_node(node, size, hash);

            // 한 번이라도 이동을 했으면
            if (before_list_ptr != NULL) {
                // 사이에 새로운 node를 삽입한다.
                before_list_ptr->next = insert_node;
                insert_node->next = list_ptr;
                break;
            } else {
                // 이동을 하지 않았으면 삽입해야 하는 노드의 크기가 가장 작은 것이므로
                // 최상단에 원소 삽입!
                insert_node->next = list_ptr;
                // 최상단을 반환함
                return insert_node;
            }
            // 그리고 종료
        }
    }
    return head;
}

// 원소가 한 개만 있는 리스트 제거
myFileListSet *remove_only_one_node(myFileListSet *head) {
    myFileListSet *list_ptr = head, *before_ptr = NULL, *tmp;
    while (list_ptr != NULL) {
        // 만약 리스트가 한 개 또는 비어 있다면
        if (list_ptr->list == NULL || list_ptr->list->next == NULL) {
            // 해당 리스트를 제거한다.
            if (before_ptr == NULL) {
                // 만약 이동하지 않았으면 헤드가 변경되어야 함.
                tmp = list_ptr->next;
                free_myFileListSet(list_ptr);
                head = tmp;
                list_ptr = head;
            } else {
                before_ptr->next = list_ptr->next;
                free_myFileListSet(list_ptr);
                list_ptr = before_ptr->next;
            }
        } else {
            // 아니면 다음칸으로 이동
            before_ptr = list_ptr;
            list_ptr = list_ptr->next;
        }
    }
    return head;
}

fileInfo *create_fileInfo(char *path, int depth, time_t mtime, time_t atime) {
    fileInfo *info = malloc(sizeof(fileInfo));
    strcpy(info->path, path);
    info->depth = depth;
    get_time(mtime, info->mtime);
    get_time(atime, info->atime);
    return info;
}

void print_myFileList(myFileList *ptr) {
    int j = 1;
    while (ptr != NULL) {
#ifdef DEBUG
        printf("(depth: %d) ", ptr->info->depth);
#endif
        printf("[%d] %s (mtime : %s) (atime : %s)\n",
               j++,
               ptr->info->path,
               ptr->info->mtime,
               ptr->info->atime
        );
        ptr = ptr->next;
    }
}

char* get_file_mtime(myFileList* ptr) {
    return ptr->info->mtime;
}

void print_myFileListSet(myFileListSet *head) {
    int i = 1;
    myFileList *ptr;
    char byte_str[BUFFER_SIZE];
    while (head != NULL) {
        get_byte_to_string(head->size, byte_str);
        printf("---- Identical files #%d (%s bytes - %s) ----\n",
               i++,
               byte_str,
               head->hash
        );
        ptr = head->list;
        print_myFileList(ptr);
        head = head->next;
        printf("\n");
    }
}
