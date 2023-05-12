/* strstr example from https://cplusplus.com/reference/cstring/strstr/ */
#include <stdio.h>
#include <string.h>
#include "nocrt.h"

int main ()
{
  char str[] = "This is a simple string";
  char *pch;
  pch = strstr(str,"simple");
  if(pch != NULL)
  {
    strncpy(pch,"sample",6);
  }
  puts(str);
  
  char str2[] = "short";
  
  pch = strstr(str2, "very long");
  printf("%p\n", pch);
  pch = strstr(str2, "short");
  printf("%d\n", pch == str2);
  pch = strstr(str2, "shortt");
  printf("%p\n", pch);
  
  return 0;
}
