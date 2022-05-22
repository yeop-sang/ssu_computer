#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <dirent.h>
#include <ctype.h>
#include <math.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#define NAMEMAX 255
#define PATHMAX 4096

#ifdef HASH_SHA1
	#define HASHMAX 41
#else
	#define HASHMAX 33
#endif

#define STRMAX 10000 
#define ARGMAX 10

typedef struct fileInfo {
    // 파일 경로
	char path[PATHMAX];
    // file stat
	struct stat statbuf;
    // file link node
	struct fileInfo *next;
} fileInfo;

// 파일 리스트
typedef struct fileList {
    // 파일 사이즈
	long long filesize;
    // hash 결과값
	char hash[HASHMAX];
    // 파일 정보
	fileInfo *fileInfoList;
    // link node
	struct fileList *next;
} fileList;

// 디렉토리 리스트 링크드 리스트
typedef struct dirList {
    // 디렉토리 절대 경로
	char dirpath[PATHMAX];
	struct dirList *next;
} dirList;

#define DIRECTORY 1
#define REGFILE 2

#define KB 1000
#define MB 1000000
#define GB 1000000000
#define KiB 1024
#define MiB 1048576
#define GiB 1073741824
#define SIZE_ERROR -2

char extension[10];
char same_size_files_dir[PATHMAX];
char trash_path[PATHMAX];
long long minbsize;
long long maxbsize;
// 중복된 애들 저장할 공간
// 0번째 공간은 항상 비워져있음!
fileList *dups_list_h;

void fileinfo_append(fileInfo *head, char *path)
// 파일 정보 삽입!
{
	fileInfo *fileinfo_cur;

	fileInfo *newinfo = (fileInfo *)malloc(sizeof(fileInfo));
	memset(newinfo, 0, sizeof(fileInfo));
	strcpy(newinfo->path, path);
	lstat(newinfo->path, &newinfo->statbuf);
	newinfo->next = NULL;

	if (head->next == NULL)
        // head가 비어 있으면 바로 삽입
		head->next = newinfo;
	else {
        // 비어져 있지 않으면 맨 뒤에 삽입!
		fileinfo_cur = head->next;
		while (fileinfo_cur->next != NULL)
			fileinfo_cur = fileinfo_cur->next;

		fileinfo_cur->next = newinfo;
	}
	
}

fileInfo *fileinfo_delete_node(fileInfo *head, char *path)
// file info list 에서 node 하나 삭제
// return 값 : 삭제된 바로 다음 노드 출력
{
	fileInfo *deleted;

	if (!strcmp(head->next->path, path)){
		deleted = head->next;
		head->next = head->next->next;
		return head->next;
	}
	else {
		fileInfo *fileinfo_cur = head->next;

		while (fileinfo_cur->next != NULL){
			if (!strcmp(fileinfo_cur->next->path, path)){
				deleted = fileinfo_cur->next;
				
				fileinfo_cur->next = fileinfo_cur->next->next;
				return fileinfo_cur->next;
			}

			fileinfo_cur = fileinfo_cur->next;
		}
	}
}

int fileinfolist_size(fileInfo *head)
{
	fileInfo *cur = head->next;
	int size = 0;
	
	while (cur != NULL){
		size++;
		cur = cur->next;
	}
	
	return size;
}

void filelist_append(fileList *head, long long filesize, char *path, char *hash)
// 새로운 파일 리스트 생성
{
    fileList *newfile = (fileList *)malloc(sizeof(fileList));
    memset(newfile, 0, sizeof(fileList));

    newfile->filesize = filesize;
    strcpy(newfile->hash, hash);

    newfile->fileInfoList = (fileInfo *)malloc(sizeof(fileInfo));
    memset(newfile->fileInfoList, 0, sizeof(fileInfo));

    fileinfo_append(newfile->fileInfoList, path);
    newfile->next = NULL;

    if (head->next == NULL) {
        // 비어 있었다면 새로운 노드를 head로 만듦
        head->next = newfile;
    }    
    else {
        fileList *cur_node = head->next, *prev_node = head, *next_node;

        // 없을때까지 or 파일 크기가 다음 것보다 작아질때까지 순회
        while (cur_node != NULL && cur_node->filesize < newfile->filesize) {
            prev_node = cur_node;
            cur_node = cur_node->next;
        }

        // 현재 것과 이전 것 사이에 삽입
        newfile->next = cur_node;
        prev_node->next = newfile;
    }    
}

void filelist_delete_node(fileList *head, char *hash)
// filelist node 삭제
{
	fileList *deleted;

    // 해쉬 정보가 일치하면 바로 삭제
	if (!strcmp(head->next->hash, hash)){
		deleted = head->next;
		head->next = head->next->next;
	}
	else {
		fileList *filelist_cur = head->next;

        // 해쉬 정보가 일치할때까지 순회함!
		while (filelist_cur->next != NULL){
			if (!strcmp(filelist_cur->next->hash, hash)){
                // 해쉬값이 일치하면
                // 일치하는 노드를 list에서 제거
				deleted = filelist_cur->next;
				
				filelist_cur->next = filelist_cur->next->next;

                // 정지
				break;
			}

			filelist_cur = filelist_cur->next;
		}
	}

    // 삭제된 노드의 memory 해제
	free(deleted);
}

int filelist_size(fileList *head)
{
	fileList *cur = head->next;
	int size = 0;
	
	while (cur != NULL){
		size++;
		cur = cur->next;
	}
	
	return size;
}

int filelist_search(fileList *head, char *hash)
/* 중복 리스트에 hash 값이 같으면 삽입할 위치를 반환하는
 * return 0:    삽입되지 않음
 *        else: idx 위치에 삽입됨
 * */
{
	fileList *cur = head;
	int idx = 0;

	while (cur != NULL){
        // 해쉬 값이 같으면 반환
		if (!strcmp(cur->hash, hash))
			return idx;
        // 같지 않으면 다음으로 넘어감
		cur = cur->next;
		idx++;
	}

	return 0;
}

void dirlist_append(dirList *head, char *path)
//
{
    // 새로운 노드 생성
	dirList *newFile = (dirList *)malloc(sizeof(dirList));

    // 파일 경로 세팅
	strcpy(newFile->dirpath, path);
	newFile->next = NULL;

    // head가 비어 있으면 새로운 노드를 head로 지정
	if (head->next == NULL)
		head->next = newFile;
	else{
        // 아니면 끝에 추가
		dirList *cur = head->next;

		while(cur->next != NULL)
			cur = cur->next;
		
		cur->next = newFile;
	}
}

void dirlist_print(dirList *head, int index)
{
	dirList *cur = head->next;
	int i = 1;

	while (cur != NULL){
		if (index) 
			printf("[%d] ", i++);
		printf("%s\n", cur->dirpath);
		cur = cur->next;
	}
}

void dirlist_delete_all(dirList *head)
{
	dirList *dirlist_cur = head->next;
	dirList *tmp;

	while (dirlist_cur != NULL){
		tmp = dirlist_cur->next;
		free(dirlist_cur);
		dirlist_cur = tmp;
	}

	head->next = NULL;
}

int tokenize(char *input, char *argv[])
{
	char *ptr = NULL;
	int argc = 0;
	ptr = strtok(input, " ");

	while (ptr != NULL){
		argv[argc++] = ptr;
		ptr = strtok(NULL, " ");
	}

	argv[argc-1][strlen(argv[argc-1])-1] = '\0';

	return argc;
}

void get_path_from_home(char *path, char *path_from_home)
{
	char path_without_home[PATHMAX] = {0,};
	char *home_path;

    home_path = getenv("HOME");

    if (strlen(path) == 1){
        // path가 "\0"이면 현재 User의 home directory를 불러온다.
		strncpy(path_from_home, home_path, strlen(home_path));
	}
    else {
        // 아니면 home_directory부터 path의 경로까지 더한다.
        strncpy(path_without_home, path + 1, strlen(path)-1);
        sprintf(path_from_home, "%s%s", home_path, path_without_home);
    }
}

int is_dir(char *target_dir)
{
    struct stat statbuf;

    if (lstat(target_dir, &statbuf) < 0){
        printf("ERROR: lstat error for %s\n", target_dir);
        return 1;
    }
    return S_ISDIR(statbuf.st_mode) ? DIRECTORY : 0;

}

long long get_size(char *filesize)
{
	double size_bytes = 0;
	char size[STRMAX] = {0, };
	char size_unit[4] = {0, };
	int size_idx = 0;

	if (!strcmp(filesize, "~"))
		size_bytes = -1;
	else {
		for (int i = 0; i < strlen(filesize); i++){
			if (isdigit(filesize[i]) || filesize[i] == '.'){
				size[size_idx++] = filesize[i];
				if (filesize[i] == '.' && !isdigit(filesize[i + 1]))
					return SIZE_ERROR;
			}
			else {
				strcpy(size_unit, filesize + i);
				break;
			}
		}

		size_bytes = atof(size);

		if (strlen(size_unit) != 0){
			if (!strcmp(size_unit, "kb") || !strcmp(size_unit, "KB"))
				size_bytes *= KB;
			else if (!strcmp(size_unit, "mb") || !strcmp(size_unit, "MB"))
				size_bytes *= MB;
			else if (!strcmp(size_unit, "gb") || !strcmp(size_unit, "GB"))
				size_bytes *= GB;
			else if (!strcmp(size_unit, "kib") || !strcmp(size_unit, "KiB"))
				size_bytes *= KiB;
			else if (!strcmp(size_unit, "mib") || !strcmp(size_unit, "MiB"))
				size_bytes *= MiB;
			else if (!strcmp(size_unit, "gib") || !strcmp(size_unit, "GiB"))
				size_bytes *= GiB;
			else
				return SIZE_ERROR;
		}
	}

	return (long long)size_bytes;
}

int get_file_mode(char *target_file, struct stat *statbuf)
// 파일의 종류 판단
{
	if (lstat(target_file, statbuf) < 0){
        printf("ERROR: lstat error for %s\n", target_file);
        return 0;
    }

    if (S_ISREG(statbuf->st_mode))
    	return REGFILE;
    else if(S_ISDIR(statbuf->st_mode))
    	return DIRECTORY;
    else
    	return 0;
}

void get_fullpath(char *target_dir, char *target_file, char *fullpath)
{
	strcat(fullpath, target_dir);

	if(fullpath[strlen(target_dir) - 1] != '/')
		strcat(fullpath, "/");

	strcat(fullpath, target_file);
	fullpath[strlen(fullpath)] = '\0';
}

int get_dirlist(char *target_dir, struct dirent ***namelist)
{
	int cnt = 0;

	if ((cnt = scandir(target_dir, namelist, NULL, alphasort)) == -1){
		printf("ERROR: scandir error for %s\n", target_dir);
		return -1;
	}

	return cnt;
}

char *get_extension(char *filename)
{
	char *tmp_ext;

	if ((tmp_ext = strstr(filename, ".tar")) != NULL || (tmp_ext = strrchr(filename, '.')) != NULL)
		return tmp_ext + 1;
	else
		return NULL;
}

void get_filename(char *path, char *filename)
{
	char tmp_name[NAMEMAX];
	char *pt = NULL;

	memset(tmp_name, 0, sizeof(tmp_name));
	
	if (strrchr(path, '/') != NULL)
		strcpy(tmp_name, strrchr(path, '/') + 1);
	else
		strcpy(tmp_name, path);
	
	if ((pt = get_extension(tmp_name)) != NULL)
		pt[-1] = '\0';

	if (strchr(path, '/') == NULL && (pt = strrchr(tmp_name, '.')) != NULL)
		pt[0] = '\0';
	
	strcpy(filename, tmp_name);
}

void get_new_file_name(char *org_filename, char *new_filename)
// 휴지통에서 겹치는 파일 이름을 생성시 변경하기 위한 함수
{
	char new_trash_path[PATHMAX];
	char tmp[NAMEMAX];
	struct dirent **namelist;
	int trashlist_cnt;
	int same_name_cnt = 1;

    // 파일명만 뽑아옴
	get_filename(org_filename, new_filename);
    // 쓰레기통에 몇 개의 파일이 있는 지 읽어옴
	trashlist_cnt = get_dirlist(trash_path, &namelist);

	for (int i = 0; i < trashlist_cnt; i++){
        // 부모나 현재 디렉토리는 제외
		if (!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, ".."))
			continue;

		memset(tmp, 0, sizeof(tmp));
        // 파일이름을 불러와서 같은게 있으면 + 1 시킨다.
		get_filename(namelist[i]->d_name, tmp);

		if (!strcmp(new_filename, tmp))
			same_name_cnt++;
	}

    // .중복되는 총 수를 붙여서 출력함
	sprintf(new_filename + strlen(new_filename), ".%d", same_name_cnt);

    // 만약 extension이 존재한다면 뒤에 붙여준다.
	if (get_extension(org_filename) != NULL)
		sprintf(new_filename + strlen(new_filename), ".%s", get_extension(org_filename));
}

void remove_files(char *dir)
// 해당하는 경로의 모든 파일 삭제
{
    // dirent list를 이용하여 dir 경로부터 모두 읽어옴
	struct dirent **namelist;
    // 읽어온 파일 갯수
	int listcnt = get_dirlist(dir, &namelist);

	for (int i = 0; i < listcnt; i++){
		char fullpath[PATHMAX] = {0, };

        // 현재 dir이나 부모 dir의 경우 무시함
		if (!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, ".."))
			continue;

        // fullpath에 현재 찾아낸 파일의 경로를 삽입함.
		get_fullpath(dir, namelist[i]->d_name, fullpath);

        // 경로에 해당하는 파일 삭제
		remove(fullpath);
	}
}

void get_same_size_files_dir(void)
{
    // home direcory부터 학번 direcotry 안의 경로를 same_size_files_dir에 저장한다.
	get_path_from_home("~/20200000", same_size_files_dir);

    // ~/2020000의 절대 경로가 접근 가능이면 내부의 모든 파일을 지운다..
	if (access(same_size_files_dir, F_OK) == 0)
		remove_files(same_size_files_dir);
	else
        // 없으면 생성한다.
		mkdir(same_size_files_dir, 0755);
}

void get_trash_path(void)
{
    // root 계정이면
	if (getuid() == 0){
        // ~/Trash 에서 가져옴
		get_path_from_home("~/Trash/", trash_path);

        // 파일이 존재하면 내부 clear
		if (access(trash_path, F_OK) == 0)
			remove_files(trash_path);
		else
            // 없으면 생성
			mkdir(trash_path, 0755);
	}
	else
        // 개인 계정이면 다른 곳에 생성
		get_path_from_home("~/.local/share/Trash/files/", trash_path);
}

int check_args(int argc, char *argv[])
// 들어온 인자 파싱
{
	char target_dir[PATHMAX];

	if (argc != 5){
		printf("Usage: find [FILE_EXTENSION] [MINSIZE] [MAXSIZE] [DIRECTORY]\n");
		return 1;
	}

	if (strchr(argv[1], '*') == NULL){
		printf("ERROR: [FILE_EXTENSION] should be '*' or '*.extension'\n");
		return 1;
	}

	if (strchr(argv[1], '.') != NULL){
		strcpy(extension, get_extension(argv[1]));

		if (strlen(extension) == 0){
			printf("ERROR: There should be extension\n");
			return 1;
		}
	}

	minbsize = get_size(argv[2]);

	if (minbsize == -1)
		minbsize = 0;

	maxbsize = get_size(argv[3]);

	if (minbsize == SIZE_ERROR || maxbsize == SIZE_ERROR){
		printf("ERROR: Size wrong -min size : %s -max size : %s\n", argv[2], argv[3]);
		return 1;
	}

	if (maxbsize != -1 && minbsize > maxbsize){
		printf("ERROR: [MAXSIZE] should be bigger than [MINSIZE]\n");
		return 1;
	}

		if (strchr(argv[4], '~') != NULL)
		get_path_from_home(argv[4], target_dir);
	else{
		if (realpath(argv[4], target_dir) == NULL){
			printf("ERROR: [TARGET_DIRECTORY] should exist\n");
			return 1;
		}
	}

	if (access(target_dir, F_OK) == -1){
		printf("ERROR: %s directory doesn't exist\n", target_dir);
		return 1;
	}

	if (!is_dir(target_dir)){
		printf("ERROR: [TARGET_DIRECTORY] should be a directory\n");
		return 1;
	}

	return 0;
}

void filesize_with_comma(long long filesize, char *filesize_w_comma)
{
	char filesize_wo_comma[STRMAX] = {0, };
	int comma;
	int idx = 0;

	sprintf(filesize_wo_comma, "%lld", filesize);
	comma = strlen(filesize_wo_comma)%3;

	for (int i = 0 ; i < strlen(filesize_wo_comma); i++){
		if (i > 0 && (i%3) == comma)
			filesize_w_comma[idx++] = ',';

		filesize_w_comma[idx++] = filesize_wo_comma[i];
	}

	filesize_w_comma[idx] = '\0';
}

void sec_to_ymdt(struct tm *time, char *ymdt)
{
	sprintf(ymdt, "%04d-%02d-%02d %02d:%02d:%02d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
}

void filelist_print_format(fileList *head)
{
	fileList *filelist_cur = head->next;
	int set_idx = 1;	

	while (filelist_cur != NULL){
		fileInfo *fileinfolist_cur = filelist_cur->fileInfoList->next;
		char mtime[STRMAX];
		char atime[STRMAX];
		char filesize_w_comma[STRMAX] = {0, };
		int i = 1;

		filesize_with_comma(filelist_cur->filesize, filesize_w_comma);

		printf("---- Identical files #%d (%s bytes - %s) ----\n", set_idx++, filesize_w_comma, filelist_cur->hash);

		while (fileinfolist_cur != NULL){
			sec_to_ymdt(localtime(&fileinfolist_cur->statbuf.st_mtime), mtime);
			sec_to_ymdt(localtime(&fileinfolist_cur->statbuf.st_atime), atime);
			printf("[%d] %s (mtime : %s) (atime : %s)\n", i++, fileinfolist_cur->path, mtime, atime);

			fileinfolist_cur = fileinfolist_cur->next;
		}

		printf("\n");

		filelist_cur = filelist_cur->next;
	}	
}

int md5(char *target_path, char *hash_result)
{
	FILE *fp;
	unsigned char hash[MD5_DIGEST_LENGTH];
	unsigned char buffer[SHRT_MAX];
	int bytes = 0;
	MD5_CTX md5;

	if ((fp = fopen(target_path, "rb")) == NULL){
		printf("ERROR: fopen error for %s\n", target_path);
		return 1;
	}

	MD5_Init(&md5);

	while ((bytes = fread(buffer, 1, SHRT_MAX, fp)) != 0)
		MD5_Update(&md5, buffer, bytes);
	
	MD5_Final(hash, &md5);

	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
		sprintf(hash_result + (i * 2), "%02x", hash[i]);
	hash_result[HASHMAX-1] = 0;

	fclose(fp);

	return 0;
}

int sha1(char *target_path, char *hash_result)
{
	FILE *fp;
	unsigned char hash[SHA_DIGEST_LENGTH];
	unsigned char buffer[SHRT_MAX];
	int bytes = 0;
	SHA_CTX sha1;

	if ((fp = fopen(target_path, "rb")) == NULL){
		printf("ERROR: fopen error for %s\n", target_path);
		return 1;
	}

	SHA1_Init(&sha1);

	while ((bytes = fread(buffer, 1, SHRT_MAX, fp)) != 0)
		SHA1_Update(&sha1, buffer, bytes);
	
	SHA1_Final(hash, &sha1);

	for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
		sprintf(hash_result + (i * 2), "%02x", hash[i]);
	hash_result[HASHMAX-1] = 0;

	fclose(fp);

	return 0;
}

void hash_func(char *path, char *hash)
{
#ifdef HASH_SHA1
	sha1(path, hash);
#else
	md5(path, hash);
#endif
}

void dir_traverse(dirList *dirlist)
// 해당 directory 탐색
{
	dirList *cur = dirlist->next;
	dirList *subdirs = (dirList *)malloc(sizeof(dirList));

	memset(subdirs, 0 , sizeof(dirList));

	while (cur != NULL){
		struct dirent **namelist;
		int listcnt;

        // 디렉토리 내부 파일들 다 불러옴
		listcnt = get_dirlist(cur->dirpath, &namelist);

		for (int i = 0; i < listcnt; i++){
			char fullpath[PATHMAX] = {0, };
			struct stat statbuf;
			int file_mode;
			long long filesize;

            // 현재 디렉토리나 부모 디렉토리는 제외
			if (!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, ".."))
				continue;

            // 절대 경로를 불러옴
			get_fullpath(cur->dirpath, namelist[i]->d_name, fullpath);

            // 만약 /proc 나 /run 이나 /sys 디렉토리면 제외
			if (!strcmp(fullpath,"/proc") || !strcmp(fullpath, "/run") || !strcmp(fullpath, "/sys") || !strcmp(fullpath, trash_path))
				continue;

            // file 형태를 판단하기 위한 정보
			file_mode = get_file_mode(fullpath, &statbuf);

            // 파일 크기가 0이면 제외
			if ((filesize = (long long)statbuf.st_size) == 0)
				continue;

            // 최소 크기보다 작아도 제외
			if (filesize < minbsize)
				continue;

            // 최대 크기보다 크면서 최대 크기가 무한이지 않은 경우 제외
			if (maxbsize != -1 && filesize > maxbsize)
				continue;

            // 디렉토리의 경우 재귀
			if (file_mode == DIRECTORY)
				dirlist_append(subdirs, fullpath);
            // regular 파일인 경우
			else if (file_mode == REGFILE){
				FILE *fp;
				char filename[PATHMAX*2];
				char *path_extension;
				char hash[HASHMAX];

                // 파일 사이즈 크기에 해당하는 파일을 생성
				sprintf(filename, "%s/%lld", same_size_files_dir, filesize);

				memset(hash, 0, HASHMAX);
                // 정해진 정보에 따라 해당 파일 해싱, hash 값 생성
				hash_func(fullpath, hash);

                // file의 확장자 가져옴
				path_extension = get_extension(fullpath);

                // 파일 확장자가 있으면
				if (strlen(extension) != 0){
                    // 입력된 파일 확장자랑 다르면 pass
					if (path_extension == NULL || strcmp(extension, path_extension))
						continue;
				}

                // 파일을 열고
				if ((fp = fopen(filename, "a")) == NULL){
					printf("ERROR: fopen error for %s\n", filename);
					return;
				}

                // 해당 파일에 hash 값하고 경로를 붙여넣음.
				fprintf(fp, "%s %s\n", hash, fullpath);

				fclose(fp);
			}
		}

		cur = cur->next;
	}

	dirlist_delete_all(dirlist);

	if (subdirs->next != NULL)
		dir_traverse(subdirs);
}

void find_duplicates(void)
{
	struct dirent **namelist;
	int listcnt;
	char hash[HASHMAX];
	FILE *fp;

    // 중복된 애들을 저장한 디렉토리를 열음
	listcnt = get_dirlist(same_size_files_dir, &namelist);

	for (int i = 0; i < listcnt; i++){
		char filename[PATHMAX*2];
		long long filesize;
		char filepath[PATHMAX];
		char hash[HASHMAX];
		char line[STRMAX];

        // 현재 디렉토리나 부모 디렉토리면 넘어감
		if (!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, ".."))
			continue;

        // 파일명이 filesize임
		filesize = atoll(namelist[i]->d_name);
		sprintf(filename, "%s/%s", same_size_files_dir, namelist[i]->d_name);

        // 파일을 열어서
		if ((fp = fopen(filename, "r")) == NULL){
			printf("ERROR: fopen error for %s\n", filename);
			continue;
		}

		while (fgets(line, sizeof(line), fp) != NULL){
			int idx;

            // 해쉬 데이터를 읽어옴
			strncpy(hash, line, HASHMAX);
			hash[HASHMAX-1] = '\0';

            // 경로 값을 읽어옴
			strcpy(filepath, line+HASHMAX);
			filepath[strlen(filepath)-1] = '\0';

            // 해쉬 값하고 중복 리스트 넘김
			if ((idx = filelist_search(dups_list_h, hash)) == 0)
                // 삽입되지 않았으면 새로운 파일 list를 생성함
				filelist_append(dups_list_h, filesize, filepath, hash);
			else{
                // 제대로 삽입할 위치를 찾았다면
				fileList *filelist_cur = dups_list_h;
                // 해당 파일 리스트로 이동해
				while (idx--){
					filelist_cur = filelist_cur->next;
				}
                // 삽입
				fileinfo_append(filelist_cur->fileInfoList, filepath);
			}
		}

		fclose(fp);
	}
}

void remove_no_duplicates(void)
{
	fileList *filelist_cur = dups_list_h->next;

	while (filelist_cur != NULL){
        // 파일 정보 가져 옴
		fileInfo *fileinfo_cur = filelist_cur->fileInfoList;

        // 1개만 있는 노드는 삭제함
		if (fileinfolist_size(fileinfo_cur) < 2)
			filelist_delete_node(dups_list_h, filelist_cur->hash);

        // 순회함
		filelist_cur = filelist_cur->next;
	}
}

time_t get_recent_mtime(fileInfo *head, char *last_filepath)
// 가장 최근에 수정된 파일 경로 가져옴
{
	fileInfo *fileinfo_cur = head->next;
	time_t mtime = 0;

	while (fileinfo_cur != NULL){
		if (fileinfo_cur->statbuf.st_mtime > mtime){
			mtime = fileinfo_cur->statbuf.st_mtime;
			strcpy(last_filepath, fileinfo_cur->path);
		}
		fileinfo_cur = fileinfo_cur->next;
	}
	return mtime;
}

void delete_prompt(void)
// 삭제 프롬프트
{
    // list 사이즈가 0이 될때까지 순회
	while (filelist_size(dups_list_h) > 0){
		char input[STRMAX];
		char last_filepath[PATHMAX];
		char modifiedtime[STRMAX];
		char *argv[3];
		int argc;
		int set_idx;
		time_t mtime = 0;
		fileList *target_filelist_p;
		fileInfo *target_infolist_p;
				
		printf(">> ");
		
		fgets(input, sizeof(input), stdin);

        // exit 입력시 종료
		if (!strcmp(input, "exit\n")){
			printf(">> Back to Prompt\n");
			break;
		}
        // 공백 입력시 재실행
		else if(!strcmp(input, "\n"))
			continue;

        // 인자 파싱
		argc = tokenize(input, argv);

        // 인자가 제대로 입력되지 않은 경우 오류 출력
		if (argc < 2 || argc > 3){
			printf("ERROR: >> [SET_INDEX] [OPTION]\n");
			continue;
		}

        // 숫자가 입력되지 않은 경우 오류
		if (!atoi(argv[0])){
			printf("ERROR: [SET_INDEX] should be a number\n");
			continue;
		}

        // 숫자가 유효하지 않은 경우 오류 출력
		if (atoi(argv[0]) < 0 || atoi(argv[0]) > filelist_size(dups_list_h)){
			printf("ERROR: [SET_INDEX] out of range\n");
			continue;
		}

		target_filelist_p = dups_list_h->next;

        // 입력된 인덱스 값 저장
		set_idx = atoi(argv[0]);

        // 해당 위치로 이동
		while (--set_idx)
			target_filelist_p = target_filelist_p->next;

        // 해당 값의 file info list 가져옴
		target_infolist_p = target_filelist_p->fileInfoList;

        // 시간 체크를 위한 값 가져옴
		mtime = get_recent_mtime(target_infolist_p, last_filepath);
		sec_to_ymdt(localtime(&mtime), modifiedtime);

        // 다시 index 값 읽어옴
		set_idx = atoi(argv[0]);

        // f 명령어에 대한 처리
        // 가장 최근에 수정한 파일 제외하고 그냥 삭제
		if (!strcmp(argv[1],"f")){
			fileInfo *tmp;
			fileInfo *deleted = target_infolist_p->next;

            // list가 끝날때까지
			while (deleted != NULL){
				tmp = deleted->next;
                // 가장 최근에 변경된 파일이면 넘어감
                if (!strcmp(deleted->path, last_filepath)){
					deleted = tmp;
					continue;
				}

                // 이외의 경우 삭제하고 memory 해제
				remove(deleted->path);
				free(deleted);
				deleted = tmp;
			}

            // list 전체 제거
			filelist_delete_node(dups_list_h, target_filelist_p->hash);

            // 남은 파일과 변경된 시간 출력
			printf("Left file in #%d : %s (%s)\n\n", atoi(argv[0]), last_filepath, modifiedtime);
		}

        // t 명령어에 대한 처리
        // 최근에 수정된 파일 이외에 모두 삭제 및 쓰레기 통으로 이동
		else if(!strcmp(argv[1],"t")){
			fileInfo *tmp;
			fileInfo *deleted = target_infolist_p->next;
			char move_to_trash[PATHMAX];
			char filename[PATHMAX];

			while (deleted != NULL){
				tmp = deleted->next;
				
				if (!strcmp(deleted->path, last_filepath)){
					deleted = tmp;
					continue;
				}

				memset(move_to_trash, 0, sizeof(move_to_trash));
				memset(filename, 0, sizeof(filename));

                // 파일 이름만 파싱해 쓰레기통 경로 생성
				sprintf(move_to_trash, "%s%s", trash_path, strrchr(deleted->path, '/') + 1);

                // 해당 파일이 이미 있으면
				if (access(move_to_trash, F_OK) == 0){
                    // 파일명.중복수.확장자 로 새로 파일 이름을 만든다.
					get_new_file_name(deleted->path, filename);

                    // 쓰레기통 경로를 붙여서 파일 경로 불러냄
					strncpy(strrchr(move_to_trash, '/') + 1, filename, strlen(filename));
				}
                // 파일이 없으면 바로 사용
				else
					strcpy(filename, strrchr(deleted->path, '/') + 1);

                // rename을 이용해 쓰레기통으로 파일 이동
				if (rename(deleted->path, move_to_trash) == -1){
					printf("ERROR: Fail to move duplicates to Trash\n");
					continue;
				}

                // 삭제 노드 memory 해제
				free(deleted);
				deleted = tmp;
			}

            // 노드 삭제, 무조건 1개만 남으므로
			filelist_delete_node(dups_list_h, target_filelist_p->hash);
			printf("All files in #%d have moved to Trash except \"%s\" (%s)\n\n", atoi(argv[0]), last_filepath, modifiedtime);
		}

        // i 명령어에 대한 처리
        // 하나씩 보여주면서 삭제할 지 물어보는 명령어
		else if(!strcmp(argv[1],"i")){
			char ans[STRMAX];
			fileInfo *fileinfo_cur = target_infolist_p->next;
			fileInfo *deleted_list = (fileInfo *)malloc(sizeof(fileInfo));
			fileInfo *tmp;
			int listcnt = fileinfolist_size(target_infolist_p);

            // 리스트 갯수만큼 순회
			while (fileinfo_cur != NULL && listcnt--){
				printf("Delete \"%s\"? [y/n] ", fileinfo_cur->path);
				memset(ans, 0, sizeof(ans));
				fgets(ans, sizeof(ans), stdin);

                // 삭제 한다고 했으면
				if (!strcmp(ans, "y\n") || !strcmp(ans, "Y\n")){
                    // 해당 경로 파일 삭제
					remove(fileinfo_cur->path);
                    // 삭제된 바로 다음 node 얻어옴
					fileinfo_cur = fileinfo_delete_node(target_infolist_p, fileinfo_cur->path);				
				}
                // 삭제 하지 않는다고 하면
				else if (!strcmp(ans, "n\n") || !strcmp(ans, "N\n"))
                    // 다음노드를 불러와 순회
					fileinfo_cur = fileinfo_cur->next;
                // 다른 값 입력시 정지
				else {
					printf("ERROR: Answer should be 'y/Y' or 'n/N'\n");
					break;
				}
			}
			printf("\n");

			if (fileinfolist_size(target_infolist_p) < 2)
				filelist_delete_node(dups_list_h, target_filelist_p->hash);

		}

        // d 명령어에 대한 처리
        // set_idx의 list_idx 삭제
		else if(!strcmp(argv[1], "d")){
			fileInfo *deleted;
			int list_idx;

			if (argv[2] == NULL || (list_idx = atoi(argv[2])) == 0){
				printf("ERROR: There should be an index\n");
				continue;
			}

			if (list_idx < 0 || list_idx > fileinfolist_size(target_infolist_p)){
				printf("ERROR: [LIST_IDX] out of range\n");
				continue;
			}

			deleted = target_infolist_p;

            // 삭제할 위치 탐색
			while (list_idx--)
				deleted = deleted->next;

            // 삭제 전 삭제 수행 명령어 수행
			printf("\"%s\" has been deleted in #%d\n\n", deleted->path, atoi(argv[0]));
            // 삭제
			remove(deleted->path);
            // list에서도 삭제
			fileinfo_delete_node(target_infolist_p, deleted->path);

            // 만약 2개 미만으로 남았으면 삭제!
			if (fileinfolist_size(target_infolist_p) < 2)
				filelist_delete_node(dups_list_h, target_filelist_p->hash);
		}

        // 다른 명령어일때 오류 처리
		else {
			printf("ERROR: Only f, t, i, d options are available\n");
			continue;
		}

        // 목록 재출력
		filelist_print_format(dups_list_h);
	}
}		