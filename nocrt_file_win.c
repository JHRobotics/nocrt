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

#ifdef NOCRT_FILE

#include <windows.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#include "nocrt.h"
#include "nocrt_mem.h"
#include "nocrt_file.h"

#define NOCRT_FILE_MAGIC 0x0C0CF11E

typedef struct _nocrt_FILE
{
	uint32_t magic;
	HANDLE   handle;
	DWORD    stdhandle;
	int      errcode;
	char     charbuf;
	int      charpresent;	
} nocrt_FILE;

nocrt_FILE nocrt_fdesc[] =
{
	{0x0C0CF11E, INVALID_HANDLE_VALUE, STD_INPUT_HANDLE,  0, 0, 0},
	{0x0C0CF11E, INVALID_HANDLE_VALUE, STD_OUTPUT_HANDLE, 0, 0, 0},
	{0x0C0CF11E, INVALID_HANDLE_VALUE, STD_ERROR_HANDLE,  0, 0, 0},
};

nocrt_FILE *nocrt_stdin  = &nocrt_fdesc[0];
nocrt_FILE *nocrt_stdout = &nocrt_fdesc[1];
nocrt_FILE *nocrt_stderr = &nocrt_fdesc[2];

nocrt_FILE *nocrt_fopen(const char *filename, const char *mode)
{
	BOOL m_read  = FALSE;
	BOOL m_write = FALSE;
	BOOL m_append = FALSE;
	BOOL m_rw    = FALSE;
	const char *p = mode;
	HANDLE hf = INVALID_HANDLE_VALUE;
	
	nocrt_FILE *mem = (nocrt_FILE*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(nocrt_FILE));
	
	while(*p != '\0')
	{
		switch(*p)
		{
			case 'w':
			case 'W':
				m_write = TRUE;
				break;
			case 'r':
			case 'R':
				m_read = TRUE;
				break;
			case 'a':
			case 'A':
				m_append = TRUE;
				break;
			case '+':
				m_rw = TRUE;
				break;
		}
		p++;
	}
	
	if(mem == NULL) return NULL;
	

	if(m_read && !m_rw)
	{
		hf = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if(m_read && m_rw)
	{
		hf = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if(m_write && !m_rw)
	{
		hf = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if(m_write && m_rw)
	{
		hf = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if(m_append)
	{
		hf = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hf != INVALID_HANDLE_VALUE)
		{
			SetFilePointer(hf, 0, NULL, FILE_END);
		}
	}
	
	if(hf == INVALID_HANDLE_VALUE)
	{
		HeapFree(GetProcessHeap(), 0, mem);
		return NULL;
	}
	
	mem->magic     = NOCRT_FILE_MAGIC;
	mem->handle    = hf;
	mem->stdhandle = 0;
	mem->errcode   = 0;
	
	return mem;
}

#define CHECK_MAGIC(_f) ((_f) != NULL && (_f)->magic == NOCRT_FILE_MAGIC)

void nocrt_fclose(nocrt_FILE *stream)
{
	if(!CHECK_MAGIC(stream)) return;	
	
	if(stream->stdhandle != 0) return;
	
	if(stream->handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(stream->handle);
	}
	
	HeapFree(GetProcessHeap(), 0, stream);
}

static BOOL nocrt_createdefault(nocrt_FILE *stream)
{
	if(!CHECK_MAGIC(stream)) return FALSE;	
	
	if(stream->stdhandle != 0 && stream->handle == INVALID_HANDLE_VALUE)
	{
		stream->handle = GetStdHandle(stream->stdhandle);
	}
	
	if(stream->handle == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	return TRUE;
}

size_t nocrt_fread(void *ptr, size_t size, size_t count, nocrt_FILE *stream)
{
	DWORD readed = 0;
	DWORD readed_extra = 0;
	size_t total = size*count;
	unsigned char *uptr = ptr;
	
	if(total == 0) return 0;		
	
	if(!nocrt_createdefault(stream)) return 0;
	
	if(stream->charpresent)
	{
		*uptr = stream->charbuf;
		stream->charpresent = 0;
		uptr++;
		
		total--;
		readed_extra++;
	}
	
	if(ReadFile(stream->handle, uptr, total, &readed, NULL))
	{
		return (readed_extra + readed) / size;
	}
	
	return 0;
}

size_t nocrt_fwrite(const void *ptr, size_t size, size_t count, nocrt_FILE *stream)
{
	DWORD writed;
	
	if(!nocrt_createdefault(stream)) return 0;
	
	if(stream->charpresent)
	{
		nocrt_fseek(stream, -1, FILE_CURRENT);
	}
	
	if(WriteFile(stream->handle, ptr, size*count, &writed, NULL))
	{
		return writed / size;
	}
	
	return 0;
}

#pragma pack(push)
#pragma pack(1)
typedef union _store64
{
	struct {
		uint32_t low;
		int32_t  high;
	} a32;
	int64_t a64;
} store64_t;
#pragma pack(pop)

int nocrt_fseek(nocrt_FILE *stream, long long offset, int origin)
{
	store64_t s;
	s.a64 = offset;
	int win_origin = 0;
	DWORD r;
	
	if(!nocrt_createdefault(stream)) return 0;
	
	switch(origin)
	{
		case SEEK_SET:
			win_origin = FILE_BEGIN;
			stream->charpresent = 0;
			break;
		case SEEK_CUR:
			win_origin = FILE_CURRENT;
			if(stream->charpresent && offset != 0)
			{
				offset--;
				stream->charpresent = 0;
			}
			break;
		case SEEK_END:
			win_origin = FILE_END;
			stream->charpresent = 0;
			break;
		default: return 2;
	}
	
	r = SetFilePointer(stream->handle, s.a32.low, (PLONG)&(s.a32.high), win_origin);
	if(r != INVALID_SET_FILE_POINTER || GetLastError() == ERROR_SUCCESS)
	{
		return 0;
	}
	
	return 1;
}

long long nocrt_ftell(nocrt_FILE *stream)
{
	DWORD r;
	
	store64_t s;
	s.a64 = 0;
	
	if(!nocrt_createdefault(stream)) return -1;
	
	r = SetFilePointer(stream->handle, s.a32.low, (PLONG)&(s.a32.high), FILE_CURRENT);
	if(r != INVALID_SET_FILE_POINTER || GetLastError() == ERROR_SUCCESS)
	{
		s.a32.low = r;
		
		if(stream->charpresent)
		{
			s.a64--;
		}
		
		return s.a64;
	}
	
	return -1;
}

int nocrt_fgetc(nocrt_FILE *stream)
{
	unsigned char dest;
	
	if(nocrt_fread(&dest, 1, 1, stream) == 1)
	{
		return dest;
	}
	
	return EOF;
}

int nocrt_fputc(int c, nocrt_FILE *stream)
{
	unsigned char dest = (unsigned char)c;
		
	if(nocrt_fwrite(&dest, 1, 1, stream) == 1)
	{
		return c;
	}
	
	return EOF;
}

char *nocrt_fgets(char* str, int num, nocrt_FILE* stream)
{
	char *ptr = str;
	
	while(num > 0)
	{
		int c = nocrt_fgetc(stream);
		if(c == EOF)
		{
			if(ptr == str)
			{
				return NULL;
			}			
			*ptr = '\0';
			break;
		}
		
		if(c == '\n')
		{
			if(num == 1)
			{
				*ptr = '\0';
			}
			else
			{
				*ptr = c;
				ptr++;
				*ptr = '\0';
			}
			break;
		}
		
		*ptr = c;
		ptr++;		
		num--;
	}
	
	return str;
}

int nocrt_fputs(const char *str, nocrt_FILE* stream)
{
	size_t to_write = nocrt_strlen(str);
	
	if(nocrt_fwrite(str, 1, to_write, stream) == to_write)
	{
		return 1;
	}
	
	return EOF;	
}


int nocrt_feof(nocrt_FILE *stream)
{
	if(!nocrt_createdefault(stream)) return 1;
		
	if(!stream->charpresent)
	{
		DWORD readed = 0;
		if(ReadFile(stream->handle, &(stream->charbuf), 1, &readed, NULL))
		{
			if(readed == 1)
			{
				stream->charpresent = 1;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}
		
	
	return 0;
}

int nocrt_ferror(nocrt_FILE * stream)
{
	return stream->errcode;
}

int nocrt_fflush(nocrt_FILE *stream)
{
	if(stream == NULL)
	{
		return -1;
	}
	
	if(!nocrt_createdefault(stream)) return 1;
		
	if(!FlushFileBuffers(stream->handle))
	{
		return 1;	
	}
	
	return 0;
}

static void nocrt_vfprintf_putc(void *resource, char c)
{
	nocrt_FILE *stream = (nocrt_FILE*)resource;
	
	nocrt_fputc(c, stream);
}

int nocrt_vfprintf(nocrt_FILE *stream, const char *format, va_list arg)
{
	return nocrt_vformatf(stream, nocrt_vfprintf_putc, ULONG_MAX, format, arg);
}

int nocrt_fprintf(nocrt_FILE *stream, const char *format, ...)
{
	int r;
	va_list arg;
	va_start(arg, format);	
	r = nocrt_vfprintf(stream, format, arg);
	va_end(arg);
	
	return r;
}

void nocrt_rewind(nocrt_FILE *stream)
{
	nocrt_fseek(stream, 0, SEEK_SET);
}

int nocrt_vprintf(const char *format, va_list arg)
{
	return nocrt_vfprintf(nocrt_stdout, format, arg);
}

int nocrt_printf(const char *format, ...)
{
	int r;
	va_list arg;
	va_start(arg, format);	
	r = nocrt_vprintf(format, arg);
	va_end(arg);
	
	return r;
}

int nocrt_puts(const char *str)
{
	int r = nocrt_fputs(str, nocrt_stdout);
	nocrt_fputc('\n', nocrt_stdout);
	
	return r;
}

int nocrt_getchar()
{
	return nocrt_fgetc(nocrt_stdin);
}

int nocrt_putchar(int character)
{
	return nocrt_fputc(character, stdout);
}

int nocrt_rename(const char *oldname, const char *newname)
{
	if(MoveFileExA(oldname, newname, MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING))
	{
		return 0;
	}
	
	return -1;
}

/* some posix filesystem functions */
int nocrt_mkdir(const char *path, int mode)
{
	(void)mode;
	
	return (CreateDirectoryA(path, NULL) != FALSE) ? 0 : -1;
}

int nocrt_unlink(const char *path)
{
	return (DeleteFileA(path) != FALSE) ? 0 : -1;
}

#endif /* NOCRT_FILE */
