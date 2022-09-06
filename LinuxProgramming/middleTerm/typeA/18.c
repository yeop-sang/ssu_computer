#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFLEN 512
#define QNUM 5
void print_wrong_answer(char* fname);

int main(int argc, char** argv) {
    
    if(argc < 2)
        exit(1);

    print_wrong_answer(argv[1]);

    exit(0);
}

void print_wrong_answer(char* filename) {
    FILE* fp;
    char question_name[QNUM][15];
    char line[BUFLEN];
    char *ptr;
    int question_num = 0;
    int error_cnt;

    if((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "fopen error for %s\n", filename);
        exit(1);
    }

    fscanf(fp, "%s\n", line);
    strcpy(question_name[question_num++], strtok(line, ","));

    while((ptr = strtok(NULL, ",")) != NULL)
        strcpy(question_name[question_num++], ptr);

    while(!feof(fp)) {
        fscanf(fp, "%s\n", line);
        printf("%s\'s wrong answer:", strtok(line, ","));
        int c = 0;
        for(int i = 0; i < question_num; i++ ){
            if((ptr = strtok(NULL, ",")) != NULL)
               if(atof(ptr) == 0) {
                   printf(" %s", question_name[i]);
                   c++;
               }
        }

        if(c == 0)
            printf(" perfect!!\n");

        else 
            printf("\nThe number of wrong answer: %d\n", c);
    }
}
