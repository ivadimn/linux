#include <stdio.h>
int main()
{
  char name[100];
  printf("What is your name? __");
  fgets(name,100, stdin);
  return printf("Hello %s\n",name);
}

