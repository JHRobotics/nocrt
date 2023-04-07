/******************************************************************************\
* MIT No Attribution                                                           *
*                                                                              *
* Copyright 2023 Jaroslav Hensl <emulator@emulace.cz>                          *
*                                                                              *
* Permission is hereby granted, free of charge, to any person obtaining a copy *
* of this software and associated documentation files (the "Software"), to deal*
* in the Software without restriction, including without limitation the rights *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell    *
* copies of the Software, and to permit persons to whom the Software is        *
* furnished to do so.                                                          *
*                                                                              *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL      *
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER   *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,*
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS       *
* IN THE SOFTWARE.                                                             *
\******************************************************************************/

#include <windows.h>
#include "nocrt.h"

/* security cookie = buffer overrun protection */
#ifdef _WIN64
#define DEFAULT_SECURITY_COOKIE 0x00002B992DDFA232ll
#else
#define DEFAULT_SECURITY_COOKIE 0xBB40E64E
#endif

DECLSPEC_SELECTANY UINT_PTR __security_cookie = DEFAULT_SECURITY_COOKIE;
DECLSPEC_SELECTANY UINT_PTR __security_cookie_complement = ~(DEFAULT_SECURITY_COOKIE);

typedef union
{
  unsigned __int64 ft_scalar;
  FILETIME ft_struct;
} FT;

void __cdecl __security_init_cookie(void);

void __cdecl
__security_init_cookie(void)
{
  UINT_PTR cookie;
  FT systime = { 0, };
  LARGE_INTEGER perfctr;

  if (__security_cookie != DEFAULT_SECURITY_COOKIE)
    {
      __security_cookie_complement = ~__security_cookie;
      return;
    }

  GetSystemTimeAsFileTime(&systime.ft_struct);
#ifdef _WIN64
  cookie = systime.ft_scalar;
#else
  cookie = systime.ft_struct.dwLowDateTime;
  cookie ^= systime.ft_struct.dwHighDateTime;
#endif

  cookie ^= GetCurrentProcessId ();
  cookie ^= GetCurrentThreadId ();
  cookie ^= GetTickCount ();

  QueryPerformanceCounter (&perfctr);
#ifdef _WIN64
  cookie ^= perfctr.QuadPart;
#else
  cookie ^= perfctr.LowPart;
  cookie ^= perfctr.HighPart;
#endif

#ifdef _WIN64
  cookie &= 0x0000ffffffffffffll;
#endif

  if (cookie == DEFAULT_SECURITY_COOKIE)
    cookie = DEFAULT_SECURITY_COOKIE + 1;
  __security_cookie = cookie;
  __security_cookie_complement = ~cookie;
}

static int  def_argc = 1;
static char *def_argv[] = {"null.exe"};

static char *main_argv_mem = NULL;
static int main_argc = 0;
static char **main_argv_vect = NULL;

#define ESCAPE_CHR '^'

static int parse_command_line(const char *cmdline, char *argv_mem, char **argv_vect)
{
	int in_quotes = 0;
	size_t j = 0;
	int argc = 0;
	
	char *next_ptr = argv_mem;
	int arg_length = 0;
	
	for(size_t i = 0; cmdline[i] != '\0'; i++)
	{
		char c = cmdline[i];
		
		if(c == ESCAPE_CHR)
		{
			if(i > 0 && cmdline[i-1] == ESCAPE_CHR)
			{
				if(argv_mem != NULL) argv_mem[j++] = c;
				arg_length++;
			}
		}
		else if(in_quotes)
		{
			if(c == in_quotes)
			{
				if(i > 0)
				{
					if(cmdline[i-1] == ESCAPE_CHR)
					{
						if(argv_mem != NULL) argv_mem[j++] = c;
						arg_length++;
					}
					else
					{
						in_quotes = 0;
					}
				}
				else
				{
					in_quotes = 0;
				}
			}
			else
			{
				if(argv_mem != NULL) argv_mem[j++] = c;
				arg_length++;
			}
		}
		else
		{
			switch(c)
			{
				case '"':
				case '\'':
					if(!(i > 0 && cmdline[i-1] == ESCAPE_CHR))
					{
						in_quotes = c;
					}
					else
					{
						if(argv_mem != NULL) argv_mem[j++] = c;
						arg_length++;
					}
					break;
				case ' ':
				case '\t':
				case '\n':
				case '\r':
					if(!(i > 0 && cmdline[i-1] == ESCAPE_CHR))
					{
						if(arg_length > 0)
						{
							if(argv_vect != NULL && argv_mem != NULL)
							{
								argv_mem[j++] = '\0';
								
								argv_vect[argc] = next_ptr;
								
								next_ptr = &(argv_mem[j]);
							}
							argc++;
						}
						arg_length = 0;
					}
					else
					{
						if(argv_mem != NULL) argv_mem[j++] = c;
					}
					break;
				default:
					if(argv_mem != NULL) argv_mem[j++] = c;
					arg_length++;
			} // switch
		}
	}
	
	if(arg_length > 0)
	{
		if(argv_vect != NULL && argv_mem != NULL)
		{
			argv_mem[j++] = '\0';						
			argv_vect[argc] = next_ptr;
			next_ptr = &(argv_mem[j]);
		}
		argc++;
	}
		
	return argc;
}

BOOL alloc_argv()
{
	const char *cmd = GetCommandLineA();
	
	if(cmd != NULL)
	{
		size_t mem_size = strlen(cmd)+1;
		main_argc = parse_command_line(cmd, NULL, NULL);
		
		if(main_argc > 0)
		{
			main_argv_mem = HeapAlloc(GetProcessHeap(), 0, mem_size);
			if(main_argv_mem == NULL)
			{
				return FALSE;
			}
			
			main_argv_vect = HeapAlloc(GetProcessHeap(), 0, main_argc * sizeof(char*));
			if(main_argv_vect == NULL)
			{
				HeapFree(GetProcessHeap(), 0, main_argv_mem);
				return FALSE;
			}
			
			parse_command_line(cmd, main_argv_mem, main_argv_vect);
			return TRUE;
		}
	}
	
	return FALSE;
}

void free_argv()
{
	if(main_argv_mem != NULL)
	{
		HeapFree(GetProcessHeap(), 0, main_argv_mem);
	}
	
	if(main_argv_vect != NULL)
	{
		HeapFree(GetProcessHeap(), 0, main_argv_vect);
	}
}

int main(int argc, char **argv);

int WinMainCRTStartup(void)
{
	__security_init_cookie();
	if(alloc_argv())
	{
		int r = main(main_argc, main_argv_vect);
		free_argv();
		return r;
	}
	
	return main(def_argc, def_argv);
}

int mainCRTStartup(void)
{
	__security_init_cookie();
	if(alloc_argv())
	{
		int r;
		AllocConsole();
		r = main(main_argc, main_argv_vect);
		free_argv();
		return r;
	}
	
	AllocConsole();
	return main(def_argc, def_argv);
}

int atexit(void (*func)(void))
{
	(void)func;
	return 0;
}
