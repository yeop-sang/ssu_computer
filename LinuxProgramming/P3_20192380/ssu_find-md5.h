#include "ssu_sfinder.h"

int fmd5(int argc, char *argv[])
{
	char target_dir[PATHMAX];
	dirList *dirlist = (dirList *)malloc(sizeof(dirList));
	
	dups_list_h = (fileList *)malloc(sizeof(fileList));

	if (check_args(argc, argv, target_dir))
		return 1;

    // 이미 check arg에서 진행되었음!
//    // ~가 입력되면 홈디렉토리로 target directory 이동
//    if (strchr(argv[4], '~') != NULL)
//        get_path_from_home(argv[4], target_dir);
//        // 아니면 realpath로 읽어옴
//    else
//        realpath(argv[4], target_dir);

	get_same_size_files_dir();

	struct timeval begin_t, end_t;

	gettimeofday(&begin_t, NULL);

    set_hash(2);

	dirlist_append(dirlist, target_dir);
	dir_traverse(dirlist);
	find_duplicates();
	remove_no_duplicates();

	gettimeofday(&end_t, NULL);

	end_t.tv_sec -= begin_t.tv_sec;

	if (end_t.tv_usec < begin_t.tv_usec){
		end_t.tv_sec--;
		end_t.tv_usec += 1000000;
	}

	end_t.tv_usec -= begin_t.tv_usec;

	if (dups_list_h->next == NULL)
		printf("No duplicates in %s\n", target_dir);
	else 
		filelist_print_format(dups_list_h);

	printf("Searching time: %ld:%06ld(sec:usec)\n\n", end_t.tv_sec, end_t.tv_usec);

	get_trash_path();

	delete_prompt();

	return 0;
}