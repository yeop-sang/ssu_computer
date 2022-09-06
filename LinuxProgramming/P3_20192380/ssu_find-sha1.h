#include "ssu_sfinder.h"

int fsha1(int argc, char *argv[]) {
    char target_dir[PATHMAX];
    dirList *dirlist = (dirList *) malloc(sizeof(dirList));

    // 파일 링크드 리스트 할당
    dups_list_h = (fileList *) malloc(sizeof(fileList));

    // 분할
    if (check_args(argc, argv, target_dir))
        return 1;

    // 이미 check arg에서 진행되었음!
//    // ~가 입력되면 홈디렉토리로 target directory 이동
//    if (strchr(argv[4], '~') != NULL)
//        get_path_from_home(argv[4], target_dir);
//        // 아니면 realpath로 읽어옴
//    else
//        realpath(argv[4], target_dir);

    // 학번에 해당하는 폴더 내부의 파일들 모두 삭제
    get_same_size_files_dir();

    // 시작시간 체크
    struct timeval begin_t, end_t;

    gettimeofday(&begin_t, NULL);

    // 기본적 설정을 위한 hash set
    set_hash(1);

    // target directory로 새로운 노드 추가
    dirlist_append(dirlist, target_dir);
    // 주어진 디렉토리를 탐색하면서 ~/20200000 안에 파일 크기에 따라서 저장함.
    dir_traverse(dirlist);
    // 중복된 파일을 찾아 정리하는 함수
    find_duplicates();
    // 한 개만 있는 중복 list 삭제
    remove_no_duplicates();

    gettimeofday(&end_t, NULL);

    end_t.tv_sec -= begin_t.tv_sec;

    if (end_t.tv_usec < begin_t.tv_usec) {
        end_t.tv_sec--;
        end_t.tv_usec += 1000000;
    }

    end_t.tv_usec -= begin_t.tv_usec;

    if (dups_list_h->next == NULL)
        printf("No duplicates in %s\n", target_dir);
        // 탐색된 list가 있으면 모든 목록 출력
    else
        filelist_print_format(dups_list_h);

    // 탐색시간 출력
    printf("Searching time: %ld:%06ld(sec:usec)\n\n", end_t.tv_sec, end_t.tv_usec);

    // 쓰레기 경로 가져옴
    get_trash_path();

    // 삭제 프롬프트 실행
    delete_prompt();

    return 0;
}