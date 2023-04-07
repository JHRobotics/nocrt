#include <stdio.h>
#include "nocrt.h"

const char test1[] = "abcdefgh";
const char test2[] = "1234";

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	FILE *fw = fopen("test5.txt", "w+");
	if(!fw)
	{
		printf("Fopen failed!\n");
		return 1;
	}
	
	fwrite(test1, 1, strlen(test1), fw);
	if(feof(fw))
	{
		printf("Yes, this is end file\n");
	}
	else
	{
		printf("no EOF, something is wrong!\n");
	}
	
	fseek(fw, -4, SEEK_CUR); // move 4 bytes back
	
	feof(fw); // feof is implement as attempt to read one byte (the byte is buffered, so next read return it + read rest)
	feof(fw); // but this internal move file position sould be invisible for runtime user!
	feof(fw);
	if(feof(fw))
	{
		printf("rech EOF, something is wrong!\n");
	}
	
	fwrite(test2, 1, strlen(test2), fw);
	
	fclose(fw);
	
	FILE *fa = fopen("test5.txt", "a");
	if(!fa)
	{
		printf("Fopen failed\n");
		return 2;
	}
	
	fputc('\n', fa);
	fwrite(test2, 1, strlen(test2), fa);
	fputc('\n', fa);
	
	fclose(fa);
	
	return 0;
}

