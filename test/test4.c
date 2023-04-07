#include <stdio.h>
#include "nocrt.h"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	int c;
	
	FILE *fr = fopen("test4_in.txt", "rb");
	
	if(!fr)
	{
		printf("Fopen failer\n");
		return 0;
	}
	else
	{
		printf("Fopen success\n");
	}
	
	while(!feof(fr))
	{
		c = fgetc(fr);
		putchar(c);
	}
	
	rewind(fr);
	printf("====\n");

	for(;;)
	{
		c = fgetc(fr);
		if(c == EOF) break;
		putchar(c);
	}
	
	rewind(fr);
	printf("====\n");
	
	fseek(fr, 0, SEEK_END);
	size_t pos = ftell(fr);
	char *ptr = NULL;
	printf("File end: %lu\n", pos);
	
	if(feof(fr))
	{
		printf("EOF seek to set\n");
		fseek(fr, 0, SEEK_SET);
	}
	
	ptr = malloc(pos);
	if(ptr == NULL)
	{
		printf("malloc FAIL!\n");
	}
	else
	{
		printf("fread: %lu\n", fread(ptr, pos, 1, fr));
		
		fwrite(ptr, 1, pos, stdout);
		
		free(ptr);
	}
	
	fclose(fr);
	
	return 0;
}
