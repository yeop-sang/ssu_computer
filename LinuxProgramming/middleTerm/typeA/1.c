#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(void) 
{
	char operator;
	FILE *fp;
	int character;
	int number = 0;

	fp =  fopen("ssu_expr.txt", "r");

    while(!feof(fp))
    {
        while(!(character = 0) &&fread(&character, 1, 1, fp) && character >= '0' && character <= '9')
            number = number * 10 + (character - '0');

        fprintf(stdout, " %d\n", number);
        number = 0;

        if(character != EOF)
        {
            operator = character == '+' || character == '*' ? character : ' ' ;
            if(operator != ' ')
            printf("Operator => %c\n", operator);
        }
    }

    fclose(fp);
    exit(0);
}
