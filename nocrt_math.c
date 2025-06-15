/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#include <math.h>
#include "nocrt.h"

#ifdef NOCRT_FLOAT

double nocrt_fmod(double x, double y)
{
  double res = 0.0;

  __asm volatile (
       "1:\tfprem\n\t"
       "fstsw   %%ax\n\t"
       "sahf\n\t"
       "jp      1b\n\t"
       "fstp    %%st(1)"
       : "=t" (res) : "0" (x), "u" (y) : "ax", "st(1)");
  return res;
}

#define nocrt_isinf(_v) (((_v)==INFINITY) || (_v)==-INFINITY)

double nocrt_modf(double value, double* iptr)
{
  double int_part = 0.0;
  /* truncate */
#if defined(_AMD64_) || defined(__x86_64__)
  __asm volatile ("subq $8, %%rsp\n"
    "fnstcw 4(%%rsp)\n"
    "movzwl 4(%%rsp), %%eax\n"
    "orb $12, %%ah\n"
    "movw %%ax, (%%rsp)\n"
    "fldcw (%%rsp)\n"
    "frndint\n"
    "fldcw 4(%%rsp)\n"
    "addq $8, %%rsp\n" : "=t" (int_part) : "0" (value) : "eax"); /* round */
#elif defined(_X86_) || defined(__i386__)
  __asm volatile (
  	"push %%eax\n"
  	"subl $8, %%esp\n"
    "fnstcw 4(%%esp)\n"
    "movzwl 4(%%esp), %%eax\n"
    "orb $12, %%ah\n"
    "movw %%ax, (%%esp)\n"
    "fldcw (%%esp)\n"
    "frndint\n"
    "fldcw 4(%%esp)\n"
    "addl $8, %%esp\n"
    "pop %%eax\n"
    : "=t" (int_part) : "0" (value) : "eax"); /* round */
#else
  int_part = trunc(value);
#endif
  if (iptr)
    *iptr = int_part;

  return (nocrt_isinf(value) ?  0.0 : value - int_part);
}

double nocrt_sin(double x)
{
	double y;
#if defined(_X86_) || defined(__i386__)
  __asm volatile (
  	"fldl %1\n"
		"fsin\n"
		"fstpl %0" : "=m"(y) : "m"(x)
	);
#else
	y = nocrt_sin_eps(x, NOCRT_DOUBLE_MAX_DIGITS);
#endif
	return y;
}

float nocrt_sinf(float x)
{
	float y;
#if defined(_X86_) || defined(__i386__)
  __asm volatile (
  	"flds %1\n"
		"fsin\n"
		"fstps %0" : "=m"(y) : "m"(x)
	);
#else
	y = nocrt_sin_eps(x, NOCRT_FLOAT_MAX_DIGITS);
#endif
	return y;
}

double nocrt_cos(double x)
{
	double y;
#if defined(_X86_) || defined(__i386__)
  __asm volatile (
  	"fldl %1\n"
		"fcos\n"
		"fstpl %0" : "=m"(y) : "m"(x)
	);
#else
	y = nocrt_cos_eps(x, NOCRT_DOUBLE_MAX_DIGITS);
#endif
	return y;
}

float nocrt_cosf(float x)
{
	float y;
#if defined(_X86_) || defined(__i386__)
  __asm volatile (
  	"flds %1\n"
		"fcos\n"
		"fstps %0" : "=m"(y) : "m"(x)
	);
#else
	y = nocrt_cos_eps(x, NOCRT_FLOAT_MAX_DIGITS);
#endif
	return y;
}

static double tan_base(double x)
{
	if(x > NOCRT_PI_2)
	{
		double r;
		double f = (x / NOCRT_PI) + 0.5;
		nocrt_modf(f, &r);
		return x - r * NOCRT_PI;
	}
	else if(x < NOCRT_PI_2)
	{
		double r;
		double f = (x / NOCRT_PI) - 0.5;
		nocrt_modf(f, &r);
		return x - r * NOCRT_PI;
	}
	
	return x;
}

#define EPS_DOUBLE 0.000000000000001d
#define EPS_FLOAT  0.0000001f

double nocrt_tan(double x)
{
	double y;
#if defined(_X86_) || defined(__i386__)
	double x1 = tan_base(x);
	
	if(x1 >= (NOCRT_PI_2-EPS_DOUBLE) && x1 <= (NOCRT_PI_2+EPS_DOUBLE))
	{
		return INFINITY;
	}

	if(x1 >= (-NOCRT_PI_2-EPS_DOUBLE) && x1 <= (-NOCRT_PI_2+EPS_DOUBLE))
	{
		return -INFINITY;
	}

  __asm volatile (
  	"fldl %1\n"
		"fptan\n"
		"fxch %%st(1)\n"
		"fstpl %0" : "=m"(y) : "m"(x1) : "st(1)"
	);
#else
	y = nocrt_tan_eps(x, NOCRT_DOUBLE_MAX_DIGITS);
#endif
	return y;
}

float nocrt_tanf(float x)
{
	float y;
#if defined(_X86_) || defined(__i386__)
	float x1 = tan_base(x);

	if(x1 >= (NOCRT_PI_2-EPS_FLOAT) && x1 <= (NOCRT_PI_2+EPS_FLOAT))
	{
		return INFINITY;
	}

	if(x1 >= (-NOCRT_PI_2-EPS_FLOAT) && x1 <= (-NOCRT_PI_2+EPS_FLOAT))
	{
		return -INFINITY;
	}

  __asm volatile (
  	"flds %1\n"
		"fptan\n"
		"fxch %%st(1)\n"
		"fstps %0" : "=m"(y) : "m"(x1) : "st(1)"
	);
#else
	y = nocrt_tan_eps(x, NOCRT_FLOAT_MAX_DIGITS);
#endif
	return y;
}

#endif
