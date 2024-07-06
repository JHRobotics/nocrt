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

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <limits.h>
#include <math.h>
#include "nocrt.h"

#ifdef NOCRT

#define ZEROES(_s) nocrt_memset(&(_s), 0, sizeof(_s))

size_t nocrt_strlen(const char *s)
{
	const char *ptr = s;
	while(*ptr != '\0') ptr++;
	return ptr - s;
}

int nocrt_strcmp(const char *s1, const char *s2)
{
	const unsigned char *p1 = (const unsigned char *)s1;
	const unsigned char *p2 = (const unsigned char *)s2;
	
	while(*p1 == *p2)
	{
		if(*p1 == '\0') break;
		p1++;
		p2++;
	}
	
	return *p1 - *p2;
}

int nocrt_strncmp(const char *s1, const char *s2, size_t num)
{
	const unsigned char *p1 = (const unsigned char *)s1;
	const unsigned char *p2 = (const unsigned char *)s2;
	
	while(num > 0 && *p1 == *p2)
	{
		if(*p1 == '\0') break;
		p1++;
		p2++;
		num--;
	}
	
	if(num == 0) return 0;
	
	return *p1 - *p2;
}

int nocrt_memcmp(const void *ptr1, const void *ptr2, size_t num)
{
	const unsigned char *p1 = (const unsigned char *)ptr1;
	const unsigned char *p2 = (const unsigned char *)ptr2;
	
	if(num == 0) return 0;
	
	while(num > 1 && *p1 == *p2)
	{
		p1++;
		p2++;
		num--;
	}
	
	return *p1 - *p2;
}

static char nocrt_case_norm(char c)
{
	if(c >= 'A' && c <= 'Z')
	{
		return (c - 'A') + 'a';
	}
	
	return c;
}

int nocrt_stricmp(const char *s1, const char *s2)
{
	const char *p1 = s1;
	const char *p2 = s2;
	
	while(nocrt_case_norm(*p1) == nocrt_case_norm(*p2))
	{
		if(*p1 == '\0') break;
		p1++;
		p2++;
	}
	
	return (unsigned char)nocrt_case_norm(*p1) - (unsigned char)nocrt_case_norm(*p2);
}

int nocrt_strnicmp(const char *s1, const char *s2, size_t num)
{
	const unsigned char *p1 = (const unsigned char *)s1;
	const unsigned char *p2 = (const unsigned char *)s2;
	
	while(num > 0 && nocrt_case_norm(*p1) == nocrt_case_norm(*p2))
	{
		if(*p1 == '\0') break;
		p1++;
		p2++;
		num--;
	}
	
	if(num == 0) return 0;
	
	return nocrt_case_norm(*p1) - nocrt_case_norm(*p2);
}

char *nocrt_strcpy(char *dst, const char *src)
{	
	char *pdst = dst;
	
	while(*src != '\0')
	{
		*pdst = *src;
		src++;
		pdst++;
	}
	
	*pdst = '\0';
	
	return dst;
}

char *nocrt_strncpy(char *dst, const char *src, size_t num)
{	
	char *pdst = dst;
	
	while(num > 0 && *src != '\0')
	{
		*pdst = *src;
		src++;
		pdst++;
		num--;
	}
	
	if(num > 0)
	{
		*pdst = '\0';
	}
	
	return dst;
}

char *nocrt_strcat(char *dst, const char *src)
{	
	size_t i = nocrt_strlen(dst);
	nocrt_strcpy(dst+i, src);
	return dst;
}

char *nocrt_strncat(char *dst, const char *src, size_t num)
{	
	char *ptr = dst + nocrt_strlen(dst);
	const char *sptr = src;
	while(num-- > 0)
	{
		if(*sptr == '\0') break;
		
		*ptr = *sptr;
		ptr++;
		sptr++;
	}
	
	*ptr = '\0';
		
	return dst;
}

void  *nocrt_memcpy(void *dst, const void *src, size_t num)
{
	if(num % sizeof(long) == 0)
	{
		num /= sizeof(long);
		unsigned long *pdst32 = dst;
		const unsigned long *psrc32 = src;
		
		while(num--) *(pdst32++) = *(psrc32++);
	}
	else
	{
		unsigned char *pdst = dst;
		const unsigned char *psrc = src;
		
		while(num--) *(pdst++) = *(psrc++);
	}
	
	return dst;
}

char *nocrt_strrchr(const char *str, int character)
{
	char *r = NULL;
	while(*str != '\0')
	{
		if(*str == character)
		{
			r = (char *)str;
		}
		str++;
	}
	
	return r;
}

char *nocrt_strchr(const char *str, int character)
{
	char *r = NULL;
	while(*str != '\0')
	{
		if(*str == character)
		{
			r = (char *)str;
			break;
		}
		str++;
	}
	
	return r;
}

char *nocrt_strstr(const char *str1, const char *str2)
{
	size_t i;
	size_t str1_len = nocrt_strlen(str1);
	size_t str2_len = nocrt_strlen(str2);
	size_t cmpmax = str1_len - str2_len;

	if(str2_len > str1_len)
	{
		return NULL;
	}
	
	for(i = 0; i <= cmpmax; i++)
	{
		if(nocrt_strncmp(str1+i, str2, str2_len) == 0)
		{
			return (char*)str1+i;
		}
	}
	
	return NULL;
}

unsigned long int nocrt_strtoul(const char* str, char** endptr, int base)
{
	unsigned long num = 0;
	const char* ptr = str;
	unsigned long digit;
	int numcnt = 0;
	
	for(; *ptr != '\0'; ptr++)
	{
		switch(*ptr)
		{
			case '0':
				if(num == 0 && base == 0)
				{
					base = 8;
				}
				/* FALLTHRU */
			case '1' ... '9':
				digit = *ptr - '0';
				break;
			case 'A' ... 'Z': 
				digit = *ptr - 'A' + 10;
				break;
			case 'x':
				if(num == 0 && (base = 8 || base == 16 || base == 0))
				{
					digit = 0;
					base = 16;
					break;
				}
				/* FALLTHRU */
			case 'a' ... 'w': 
			case 'y' ... 'z':
				digit = *ptr - 'a' + 10;
				break;				
			case '-':
			case '+':
				numcnt++;
				continue;
			default:
				if(nocrt_isspace(*ptr) && numcnt == 0)
				{
					continue;
				}
				goto strtoul_dingo;
		}
		
		numcnt++;

		if(digit != 0)
		{
			if(base == 0)
			{
				base = 10;
			}
		}
		
		if((int)digit < base)
		{
			num *= base;
			num += digit;
		}
		else
		{
			break;
		}
	}
strtoul_dingo:
	
	if(endptr != NULL)
	{
		*endptr = (char*)ptr;
	}
	
	return num;
}

long int nocrt_strtol(const char* str, char** endptr, int base)
{
	unsigned long num = 0;
	const char* ptr = str;
	unsigned long digit;
	int negative = 0;
	int numcnt = 0;
	
	for(; *ptr != '\0'; ptr++)
	{
		switch(*ptr)
		{
			case '0':
				if(num == 0 && base == 0)
				{
					base = 8;
				}
				/* FALLTHRU */
			case '1' ... '9':
				digit = *ptr - '0';
				break;
			case 'A' ... 'Z': 
				digit = *ptr - 'A' + 10;
				break;
			case 'x':
				if(num == 0 && (base = 8 || base == 16 || base == 0))
				{
					digit = 0;
					base = 16;
					break;
				}
				/* FALLTHRU */
			case 'a' ... 'w': 
			case 'y' ... 'z':
				digit = *ptr - 'a' + 10;
				break;				
			case '-':
				numcnt++;
				negative = 1;
				continue;
			case '+':
				numcnt++;
				continue;
			default:
				if(nocrt_isspace(*ptr) && numcnt == 0)
				{
					continue;
				}
				goto strtol_dingo;
		}
		numcnt++;
		
		if(digit != 0)
		{
			if(base == 0)
			{
				base = 10;
			}
		}
		
		if((int)digit < base)
		{
			num *= base;
			num += digit;
		}
		else
		{
			break;
		}
	}
strtol_dingo:
	if(negative)
	{
		num = ~num + 1;
	}
	
	if(endptr != NULL)
	{
		*endptr = (char*)ptr;
	}
	
	return num;
}

long int nocrt_atol(const char *str)
{
	return nocrt_strtol(str, NULL, 10);
}

int nocrt_atoi(const char *str)
{
	return (int)nocrt_strtol(str, NULL, 10);
}

#define FMT_LEN_NORMAL 0
#define FMT_LEN_HALF -1
#define FMT_LEN_LONG  1
#define FMT_LEN_LONGLONG 2
#define FMT_LEN_TURBO 100

typedef struct _fmt_spec
{
	int dec_point;
	int width;
	int width_dec;
	int length;
	int leftalign;
	int sign_force;
	int sign_pad;
	int num_prefix;
	int zerofill;
} fmt_spec_t;

typedef unsigned long long maxint_t;
typedef signed long long maxsint_t;
#define MAXINT_SIGN (((unsigned long long)LLONG_MAX)+1)

#define MAXINT_BUFFER 128+2+1

static size_t vformat_dec(void *resource, formatf_callback_t f, size_t *pn, fmt_spec_t *spec, char type, va_list *args)
{
	static char buf[MAXINT_BUFFER];
	static const char alphabet[] = "0123456789abcdef";
	static const char Alphabet[] = "0123456789ABCDEF";
	
	const char *alpha = alphabet;
	
	maxint_t v = 0;
	int is_sign = 0;
	int is_negative = 0;
	maxint_t base = 10;
	size_t buf_pos = 0;
	
	switch(type)
	{
		case 'i':
		case 'd':
			is_sign = 1;
			base = 10;
			break;
		case 'u': base = 10; break;
  	case 'o': base = 8; break;
  	case 'x': base = 16; break;
  	case 'X': base = 16; alpha = Alphabet; break;
  	case 'b': base =  2; break;
	}
	
	if(spec->length <= -2)
	{
		if(is_sign) v = (maxint_t)((maxsint_t)va_arg(*args, int));
		else        v = va_arg(*args, int);
	}
	else if(spec->length == -1)
	{
		if(is_sign) v = (maxint_t)((maxsint_t)va_arg(*args, int));
		else        v = va_arg(*args, int);
	}
	else if(spec->length == 0)
	{
		if(is_sign) v = (maxint_t)((maxsint_t)va_arg(*args, int));
		else        v = va_arg(*args, unsigned int);
	}
	else if(spec->length == 1)
	{
		if(is_sign) v = (maxint_t)((maxsint_t)va_arg(*args, long));
		else        v = va_arg(*args, unsigned long);
	}
	else
	{
		if(is_sign) v = (maxint_t)((maxsint_t)va_arg(*args, long long));
		else        v = va_arg(*args, unsigned long long);
	}
	
	if(is_sign)
	{
		if((v & MAXINT_SIGN) != 0)
		{
			is_negative = 1;
			v = (~v)+1;
		}
	}
	
	if(spec->zerofill)
	{
		nocrt_memset(buf, '0', MAXINT_BUFFER);
	}
	else
	{
		nocrt_memset(buf, ' ', MAXINT_BUFFER);
	}
	
	if(v == 0)
	{
		buf[buf_pos++] = '0';
	}
	
	while(v)
	{
		buf[buf_pos] = alpha[v % base];
		v /= base;
		buf_pos++;
	}
	
	if(spec->width > 0)
	{
		if((size_t)spec->width > buf_pos)
		{
			buf_pos = spec->width;
		}
	}
	
	if(spec->num_prefix)
	{
		switch(type)
		{
			case 'X':
				buf[buf_pos++] = 'X';
				buf[buf_pos++] = '0';
				break;
			case 'x':
				buf[buf_pos++] = 'x';
				buf[buf_pos++] = '0';
				break;
			case 'o':
				buf[buf_pos++] = '0';
				break;
			case 'b':
				buf[buf_pos++] = 'b';
				buf[buf_pos++] = '0';
				break;
		}
	}
	
	if(is_negative)
	{
		buf[buf_pos++] = '-';
	}
	else if(spec->sign_force)
	{
		buf[buf_pos++] = '+';
	}
	else if(spec->sign_pad)
	{
		buf[buf_pos++] = ' ';
	}
	
	for(size_t i = buf_pos; i > 0; i--)
	{
		if((*pn) == 0) break;		
		
		f(resource, buf[i-1]);
		
		(*pn)--;
	}
	
	return buf_pos;
}

typedef double maxfloat_t;
#define MAXFLOAT_BUFFER 64

static size_t vformat_float(void *resource, formatf_callback_t f, size_t *pn, fmt_spec_t *spec, char type, va_list *args)
{
	static char bufup[MAXFLOAT_BUFFER];
	static char bufdown[MAXFLOAT_BUFFER];
	size_t posup = 0;
	size_t posdown = 0;
	int negative = 0;
	size_t r = 0;
	(void)type;
	
	maxfloat_t v, v1, v2, rf = 0.5;
	
	if(spec->length < 0)
	{
		v = va_arg(*args, double); // float are stored as double for variable length arguments 
	}
	else if(spec->length >= FMT_LEN_TURBO)
	{
		v = va_arg(*args, long double);
	}
	else
	{
		v = va_arg(*args, double);
	}

#ifdef NOCRT_FLOAT	
	int width_dec = spec->width_dec;
	if(width_dec == 0) width_dec = 6;
		
	if(v < 0.0f)
	{
		negative = 1;
		v = 0 - v;
	}
	
	/* round last decimal place */
	for(int i = 0; i < width_dec; i++)
	{
		rf /= 10;
	}
	v += rf;
	
	if(v < 1.0f)
	{
		bufup[posup++] = '0';
	}
	
	v2 = nocrt_modf(v, &v1);
	
	while(v1 >= 1 && posup < MAXFLOAT_BUFFER)
	{
		unsigned int d = (unsigned int)nocrt_fmod(v1, 10);
		bufup[posup] = '0' + d;
		posup++;
		v1 /= 10.0f;
	}
	
	for(int i = 0; i < width_dec && posdown < MAXFLOAT_BUFFER; i++)
	{
		maxfloat_t d;
		v2 = nocrt_modf(v2*10.0f, &d);
		bufdown[posdown] = '0' + ((unsigned int)d);
		posdown++;
	}
	
	/* print sign */
	if(negative)
	{
		if(*pn > 0)
		{
			f(resource, '-');
			(*pn)--;
		}
		r += 1;
	}
	else if(spec->sign_force)
	{
		if(*pn > 0)
		{
			f(resource, '+');
			(*pn)--;
		}
		r += 1;
	}
	else if(spec->sign_pad)
	{
		if(*pn > 0)
		{
			f(resource, ' ');
			(*pn)--;
		}
		r += 1;
	}
	
	/* print digits before decimal point */
	for(size_t i = posup; i > 0; i--)
	{
		if(*pn == 0) break;
		
		f(resource, bufup[i-1]);
		
		(*pn)--;
	}
	r += posup;
	
	/* print decimal point */
	if(width_dec != 0)
	{
		r += 1;
		
		if(*pn > 0)
		{
			f(resource, '.');
			(*pn)--;
		}
	}
	
	/* print digits after */
	for(size_t i = 0; i < posdown; i++)
	{
		if(*pn == 0) break;
		
		f(resource, bufdown[i]);
		
		(*pn)--;
	}
	r += posdown;
#endif /* NOCRT_FLOAT */
	
	return r;
}

static const char vformatf_nullstr[] = "(null)";

size_t nocrt_vformatf(void *resource, formatf_callback_t f, size_t n, const char *fmt, va_list args)
{
  int ctrl = 0; 
  size_t left = n;
  size_t cnt = 0;
  fmt_spec_t spec;
  ZEROES(spec);
  
  for(const char *ptr = fmt; *ptr != '\0'; ptr++)
  {
  	if(ctrl)
  	{
  		switch(*ptr)
  		{
  			/* final format specifier */
  			case 'i':
  			case 'd':
  			case 'u':
  			case 'o':
  			case 'x':
  			case 'X':
  			case 'b': /* ext: binary */
  				cnt += vformat_dec(resource, f, &left, &spec, *ptr, &args);
  				ctrl = 0;
  				ZEROES(spec);
  				break;
  			case 'f':
  			case 'F':
  			case 'e':
  			case 'E':
  			case 'g':
  			case 'G':
  				cnt += vformat_float(resource, f, &left, &spec, *ptr, &args);
  				ctrl = 0;
  				ZEROES(spec);
  				break;
  			case 'c':
  				if(spec.length > 0)
  				{
  					/*wchar_t junk = */va_arg(args, int);
  					/* TODO: wchar */
  				}
  				else
  				{
  					char c = va_arg(args, int);
  					if(left){f(resource, c); left--;}
  					cnt++;
  					ctrl = 0;
  					ZEROES(spec);
  				}
  				break;
  			case 's':
  				if(spec.length > 0)
  				{
  					/*wchar_t *junk = */va_arg(args, const wchar_t*);
  					/* TODO: wchar */
  				}
  				else
  				{
  					const char *ptr = va_arg(args, const char*);
  					size_t ptr_len;
  					if(ptr == NULL)
  					{
  						ptr = vformatf_nullstr;
  					}
  					
  					ptr_len = nocrt_strlen(ptr);
  					
  					/* right align of string */
  					if(!spec.leftalign && ptr_len < (size_t)spec.width)
  					{
  						for(size_t i = 0; i < spec.width-ptr_len; i++)
  						{
  							if(left){f(resource, ' '); left--;}
  							cnt++;
  						}
  					}
  					
  					/* out the string */
  					for(size_t i = 0; i < ptr_len; i++)
  					{
  						if(left){f(resource, ptr[i]); left--;}
  						cnt++;
  					}
  					
  					/* left align of string */
  					if(spec.leftalign && ptr_len < (size_t)spec.width)
  					{
  						for(size_t i = 0; i < spec.width-ptr_len; i++)
  						{
  							if(left){f(resource, ' '); left--;}
  							cnt++;
  						}
  					}
  				}
  				ctrl = 0;
  				ZEROES(spec);
  				break;
  			case 'p':
  				if(sizeof(void*) == 4)
  				{
  					spec.length = FMT_LEN_LONG;
  				}
  				else if(sizeof(void*) == 8)
  				{
  					spec.length = FMT_LEN_LONGLONG;
  				}
  				cnt += vformat_dec(resource, f, &left, &spec, 'x', &args);
  				ctrl = 0;
  				ZEROES(spec);
  				break;
  			case '%':
  				if(left){f(resource, '%'); left--;}
  				cnt++;
  				ctrl = 0;
  				ZEROES(spec);
  				break;
  			case 'n':
  				/* use number function to calculate number size but with no output */
  				vformat_dec(resource, f, 0, &spec, *ptr, &args);
  				ctrl = 0;
  				ZEROES(spec);
  				break;
  			/* flags */
  			case '-': spec.leftalign   = 1; break;
  			case '+': spec.sign_force = 1; break;
  			case ' ': spec.sign_pad   = 1; break;
  			case '#': spec.num_prefix = 1; break;
  			 /* length */
  			case 'h': spec.length += FMT_LEN_HALF; break;
  			case 'l': spec.length += FMT_LEN_LONG; break;
  			case 'z': spec.length += FMT_LEN_LONGLONG; break;
  			case 'L': spec.length += FMT_LEN_TURBO; break;
  			/* width / flag */
  			case '0':
  				if(spec.dec_point == 0 && spec.width == 0)
  				{
  					spec.zerofill = 1;
  					break;
  				}
  				/* FALLTHRU */
  			/* width */
  			case '1' ... '9':
  				if(spec.dec_point)
  				{
  					spec.width_dec *= 10;
  					spec.width_dec += *ptr - '0';
  				}
  				else
  				{
  					spec.width *= 10;
  					spec.width += *ptr - '0';
  				}
  				break;
  			/* precision */
  			case '.': spec.dec_point = 1; break;
  			/* width from args */
  			case '*':
  				if(spec.dec_point)
  				{
  					spec.width_dec = va_arg(args, int);
  				}
  				else
  				{
  					spec.width = va_arg(args, int);
  				}
  				break;
  		}
  	}
  	else
  	{
  		if(*ptr != '%')
  		{
  			if(left){f(resource, *ptr); left--;}
  			cnt++;
  		}
  		else
  		{
  			ctrl = 1;
  		}
  	}
  }
  
  return cnt;
}

static void nocrt_vsprintf_putc(void *res, char c)
{
	char **pptr = (char **)res;
	
	**pptr = c;
	(*pptr)++;
}

int nocrt_vsnprintf(char *str, size_t n, const char *format, va_list arg)
{
	char *ptr = str;
	
  size_t pos = 0;
  
  if(str == NULL)
  {
  	pos = nocrt_vformatf(&ptr, nocrt_vsprintf_putc, 0, format, arg);
  }
  else
  {
  	pos = nocrt_vformatf(&ptr, nocrt_vsprintf_putc, n, format, arg);
  	size_t term = NOCRT_MIN(pos, n-1);
  	str[term] = '\0';
  }
  
  return (int)pos;
}

int nocrt_vsprintf(char *str, const char *format, va_list arg)
{
	return nocrt_vsnprintf(str, ULONG_MAX, format, arg);
}

int nocrt_sprintf(char *str, const char *format, ...)
{
	int res = 0;
	va_list vl;
  va_start(vl, format);
  res = nocrt_vsprintf(str, format, vl);
  va_end(vl);

  return res;
}

/* all ctype function ignores locale and works as locale "C" */
int nocrt_tolower(int c)
{
	if(c >= 'A' && c <= 'Z')
	{
		return (c - 'A') + 'a';
	}
	
	return c;
}

int nocrt_toupper(int c)
{
	if(c >= 'a' && c <= 'z')
	{
		return (c - 'a') + 'A';
	}
	
	return c;
}

int nocrt_isdigit(int c)
{
	if(c >= '0' && c <= '9')
	{
		return 1;
	}
	
	return 0;
}

int nocrt_isxdigit(int c)
{
	if(c >= '0' && c <= '9')
	{
		return 1;
	}
	
	if(c >= 'A' && c <= 'F')
	{
		return 1;
	}
	
	if(c >= 'a' && c <= 'f')
	{
		return 1;
	}
	
	return 0;
}

int nocrt_isspace(int c)
{
	switch(c)
	{
		case ' ':
		case '\t':
		case '\n':
		case '\v':
		case '\f':
		case '\r':
			return 1;
	}
	
	return 0;
}

#define MEMSET_DEF(_fname) \
	void  *_fname(void *ptr, int value, size_t num){ \
	unsigned char *pdst = ptr; \
	while(num--) *(pdst++) = value; \
	return ptr; }

MEMSET_DEF(nocrt_memset)

#ifdef NOCRT_BUILD_MEMSET

#ifdef memset
#undef memset
#endif

MEMSET_DEF(memset)
#endif

#endif /* NOCRT */

