#include <stdlib.h>
#include <stdio.h>

int main()
{
  /*
  printf("reading out the shell value: %s\n", getenv("P_START"));
  setenv("P_START", "100", 1);
  */

  int i;

  for (i = 0; i < 100000; )
    i = i + 1;

  return 0;
}
