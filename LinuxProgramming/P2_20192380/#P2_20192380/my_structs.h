#ifndef P2_20192380_MY_STRUCTS_H
#define P2_20192380_MY_STRUCTS_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hash.h"

typedef struct fileInfo {
    char path[PATH_MAX];
    int depth;
    char mtime[TIME_LENGTH];
    char atime[TIME_LENGTH];
} fileInfo;

typedef struct myDirQueue {
    char dir_path[PATH_MAX];
    int depth;
    struct myDirQueue *next;
} myDirQueue;

typedef struct myFileList {
    fileInfo *info;
    struct myFileList *next;
} myFileList;

typedef struct myFileListSet {
    myFileList *list;
    struct myFileListSet *next;
    char hash[MD5_DIGEST_LENGTH > SHA_DIGEST_LENGTH ? MD5_DIGEST_LENGTH * 2 + 1 : SHA_DIGEST_LENGTH * 2 + 1];
    size_t size;
} myFileListSet;

myDirQueue *create_myDirQueue_node(char *dir_path, int depth);

myFileList *create_file_linked_node(fileInfo *info);

void enqueue_myDirQueue(myDirQueue **head, myDirQueue *node);

int is_myDirQueue_empty(myDirQueue *head);

void pop_front_myDirQueue(myDirQueue **head, char *result, int *depth);

//myFileLinkedList* insert_file_linked_list(myFileLinkedList *head, myFileLinkedList *node);

myFileListSet *insert_myFileListLinkedList(myFileListSet *head, myFileList *node, size_t size, char *hash);

fileInfo *create_fileInfo(char *path, int depth, time_t mtime, time_t atime);

void print_myFileListSet(myFileListSet *head);

void print_myFileList(myFileList *ptr);

myFileListSet *remove_only_one_node(myFileListSet *head);

void free_myFileLinkedList(myFileList *node);

char* get_file_mtime(myFileList* ptr);

#endif //P2_20192380_MY_STRUCTS_H
