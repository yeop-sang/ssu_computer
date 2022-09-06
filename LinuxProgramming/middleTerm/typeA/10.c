#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 1024
int main(void)
{
  char character;
  char buf[BUFFER_SIZE];
  // getchar(), putchar() 사용
  while ((character = getchar()) != '$')
    if (putchar(character) == '$')
    {
      fprintf(stderr, "standard output error\n");
      exit(1);
    }
  // getc(), putc() 사용
  while ((character = getc(stdin)) != '$')
    if (putc(character, stdout) == '$')
    {
      fprintf(stderr, "standard output error\n");
      exit(1);
    }
  if (ferror(stdin))
  {
    fprintf(stderr, "standard input error\n");
    exit(1);
  }
  // fgets(), fputs() 사용
  while (fgets(buf, BUFFER_SIZE, stdin) != NULL)
    if (fputs(buf, stdout) == '$')
    {
      fprintf(stderr, "standard output error\n");
      exit(1);
    }
  if (ferror(stdin))
  {
    fprintf(stderr, "standard input error\n");
    exit(1);
  }
  memset(buf, 0, sizeof(buf));
  // scanf(), printf() 사용
  scanf(" %[^\n]", buf);
  printf("%s\n", buf);
  exit(0);
}