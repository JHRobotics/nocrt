#include <stdio.h>
#include "nocrt.h"

#define BUF_LEN 32

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	char buf[BUF_LEN];
	int i = 0;
	int c;
	
	printf("Repeater, empty line to exit\n");
	do
	{
		printf("Type something: ");
		do
		{
			c = getchar();
			if(i < BUF_LEN-1)
			{
				if(c != '\r')
				{
					buf[i++] = c;
				}
			}
		} while(c != '\n' && c != EOF);
		
		if(c == EOF)
		{
			buf[i-1] = '\0';
		}
		else
		{
			buf[i] = '\0';
		}
		
		printf("You typed (%d): %s", strlen(buf), buf);
		i = 0;
		
	} while(c != EOF && strlen(buf) > 1);
	
	return 0;
}
