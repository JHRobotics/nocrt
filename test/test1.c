#include <stdio.h>
#include "nocrt.h"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	printf("Hello %s!\n", "world");
	
	printf("Hello numbers: %d %x %o\n", 123, 124, 125);
	
	printf("More format test: %04d %4x %#2o %016b\n", 123, 123, 123, 123);
	
	printf("Floats: %f, %e, %g\n", 0.6, 123.0, 1.0/3);


  /* example from https://cplusplus.com/reference/cstring/strncat/ */
  char str1[20];
  char str2[20];
  
  strcpy(str1, "To be ");
  strcpy(str2,"or not to be");
  strncat(str1, str2, 6);
  puts(str1);
	
	return 0;
}
