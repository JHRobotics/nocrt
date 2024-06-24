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

#ifndef __NOCRT_FILE_H__INCLUDED__

#ifdef NOCRT

struct _nocrt_FILE;
typedef struct _nocrt_FILE nocrt_FILE;

nocrt_FILE *nocrt_fopen(const char *fn, const char *mode);
void nocrt_fclose(nocrt_FILE *stream);
size_t nocrt_fread(void *ptr, size_t size, size_t count, nocrt_FILE *stream);
size_t nocrt_fwrite(const void * ptr, size_t size, size_t count, nocrt_FILE *stream);
int nocrt_fseek(nocrt_FILE *stream, long long offset, int origin);
long long nocrt_ftell(nocrt_FILE *stream);
int nocrt_fgetc(nocrt_FILE *stream);
int nocrt_fputc(int c, nocrt_FILE *stream);
char *nocrt_fgets(char* str, int num, nocrt_FILE* stream);
int nocrt_fputs(const char *str, nocrt_FILE* stream);
int nocrt_vfprintf(nocrt_FILE *stream, const char *format, va_list arg);
int nocrt_fprintf(nocrt_FILE *stream, const char *format, ...);
void nocrt_rewind(nocrt_FILE *stream);
int nocrt_feof(nocrt_FILE *stream);
int nocrt_ferror(nocrt_FILE * stream);
int nocrt_fflush(nocrt_FILE *stream);

int nocrt_vprintf(const char *format, va_list arg);
int nocrt_printf(const char *format, ...);
int nocrt_puts(const char *str);
int nocrt_getchar();
int nocrt_putchar(int character);

int nocrt_rename(const char *oldname, const char *newname);
int nocrt_mkdir(const char *path, int mode);
int nocrt_unlink(const char *path);

extern nocrt_FILE *nocrt_stdin;
extern nocrt_FILE *nocrt_stdout;
extern nocrt_FILE *nocrt_stderr;

#define fopen nocrt_fopen
#define fclose nocrt_fclose
#define fread nocrt_fread
#define fwrite nocrt_fwrite
#define fseek nocrt_fseek
#define ftell nocrt_ftell
#define fgetc nocrt_fgetc
#define fputc nocrt_fputc
#define fgets nocrt_fgets
#define fputs nocrt_fputs
#define vfprintf nocrt_vfprintf
#define fprintf nocrt_fprintf
#define rewind nocrt_rewind
#define vprintf nocrt_vprintf
#define printf nocrt_printf
#define puts nocrt_puts
#define getchar nocrt_getchar
#define putchar nocrt_putchar
#define feof nocrt_feof
#define ferror nocrt_ferror
#define fflush nocrt_fflush

#define rename nocrt_rename
#define mkdir  nocrt_mkdir
#define _mkdir(_d) nocrt_mkdir(_d, 0)
#define unlink nocrt_unlink


#define FILE nocrt_FILE

#ifdef stdin
#undef stdin
#endif
#define stdin  nocrt_stdin


#ifdef stdout
#undef stdout
#endif
#define stdout nocrt_stdout

#ifdef stderr
#undef stderr
#endif
#define stderr nocrt_stderr

#ifndef SEEK_SET
#define SEEK_SET 0
#endif

#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif

#ifndef SEEK_END
#define SEEK_END 2
#endif

#endif /* NOCRT */

#endif /* __NOCRT_FILE_H__INCLUDED__ */
