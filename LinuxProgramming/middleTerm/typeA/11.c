#include <stdio.h>
#include <stdlib.h>

typedef struct _person {
    char name[10];
    int age;
    double height;
} Person;

int main(void)
{
    FILE* fp;
    int i,res;

    Person ary[3] = {{"Hong GD", 500, 175.4},
        {"Lee SS", 350,180.0},
        {"King SJ", 500, 178.6}};

    Person tmp;

    //3, 4
    char* fname = "ftest.txt";
    if((fp = fopen(fname, "w")) < 0)
        fprintf(stderr, "opening %s is error", fname);

    fwrite(ary, sizeof(Person), 3, fp);

    fclose(fp);

    // 5

    if((fp = fopen(fname, "r")) < 0)
        fprintf(stderr, "opening %s is error 2", fname);

    printf("[ First print]\n");

    while(!feof(fp)) {
        fread(&tmp, sizeof(Person),1, fp);
        printf("%s %d %.2f\n", tmp.name, tmp.age, tmp.height);
    }

    fseek(fp, 0L, SEEK_SET);

    printf("[ Second print]\n");

    while(!feof(fp)) {
        fread(&tmp, sizeof(Person),1, fp);
        printf("%s %d %.2f\n", tmp.name, tmp.age, tmp.height);
    }

   
    fclose(fp);

    exit(0);
}

