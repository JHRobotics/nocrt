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

#ifndef __NOCRT_H__INCLUDED__
#define __NOCRT_H__INCLUDED__

#ifdef NOCRT

int nocrt_strcmp(const char *s1, const char *s2);
int nocrt_stricmp(const char *s1, const char *s2);
int nocrt_strnicmp(const char *s1, const char *s2, size_t num);
char *nocrt_strcpy(char *dst, const char *src);
char *nocrt_strcat(char *dst, const char *src);
char *nocrt_strncat(char *dst, const char *src, size_t num);
void  *nocrt_memcpy(void *dst, const void *src, size_t num);
void  *nocrt_memset(void *ptr, int value, size_t num);
char *nocrt_strrchr(const char *str, int character);
char *nocrt_strchr(const char *str, int character);
int nocrt_vsprintf(char *str, const char *format, va_list arg);
int nocrt_sprintf(char *str, const char *format, ...);
int nocrt_strncmp(const char *s1, const char *s2, size_t num);
size_t nocrt_strlen(const char *s);
char *nocrt_strncpy(char *dst, const char *src, size_t num);
char *nocrt_strstr(const char *str1, const char *str2);
unsigned long int nocrt_strtoul(const char* str, char** endptr, int base);
typedef void (*formatf_callback_t)(void *resource, char c);
size_t nocrt_vformatf(void *resource, formatf_callback_t f, size_t n, const char *fmt, va_list args);
int nocrt_memcmp(const void *ptr1, const void *ptr2, size_t num);
int nocrt_tolower(int c);
int nocrt_toupper(int c);
int nocrt_isdigit(int c);
int nocrt_isxdigit(int c);
int nocrt_isspace(int c);
long int nocrt_strtol(const char* str, char** endptr, int base);
long int nocrt_atol(const char *str);
int nocrt_atoi(const char *str);

#define strcmp  nocrt_strcmp
#define stricmp nocrt_stricmp
#define strnicmp nocrt_strnicmp
#define strncmp nocrt_strncmp
#define strcpy  nocrt_strcpy
#define strcat  nocrt_strcat
#define strncat nocrt_strncat
#define memcpy  nocrt_memcpy
#define memset  nocrt_memset
#define strrchr nocrt_strrchr
#define strchr  nocrt_strchr
#define vsprintf nocrt_vsprintf
#define sprintf  nocrt_sprintf
#define strlen   nocrt_strlen
#define strncpy  nocrt_strncpy
#define strstr   nocrt_strstr
#define strtoul  nocrt_strtoul
#define memcmp   nocrt_memcmp
#define tolower  nocrt_tolower
#define toupper  nocrt_toupper
#define isdigit  nocrt_isdigit
#define isxdigit nocrt_isxdigit
#define isspace  nocrt_isspace
#define strtol   nocrt_strtol
#define atoi     nocrt_atoi
#define atol     nocrt_atol

#ifdef main
#undef main
#endif

#ifdef ceil
#undef ceil
#endif

#define ceil(_f) (_f + 0.5)

#endif

#define NOCRT_MIN(_a, _b) (((_b) < (_a)) ? (_b) : (_a))
#define NOCRT_MAX(_a, _b) (((_b) > (_a)) ? (_b) : (_a))

#ifdef NOCRT_FLOAT
 double nocrt_fmod(double x, double y);
 double nocrt_modf(double value, double* iptr);
#endif

#ifdef NOCRT_CALC
	float nocrt_logf(float x);
	float nocrt_sinf(float x);
	float nocrt_cosf(float x);
	float nocrt_tanf(float x);
	double nocrt_log(double x);
	double nocrt_sin(double x);
	double nocrt_cos(double x);
	double nocrt_tan(double x);

# define logf nocrt_logf
# define sinf nocrt_sinf
# define cosf nocrt_cosf
# define tanf nocrt_tanf
# define log nocrt_log
# define sin nocrt_sin
# define cos nocrt_cos
# define tan nocrt_tan
#endif

#ifdef NOCRT_MEM
# include "nocrt_mem.h"
#endif

#ifdef NOCRT_FILE
# include "nocrt_file.h"
#endif

#endif
