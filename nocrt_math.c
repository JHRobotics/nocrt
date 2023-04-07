/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#include <math.h>

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
  __asm volatile ("push %%eax\n\tsubl $8, %%esp\n"
    "fnstcw 4(%%esp)\n"
    "movzwl 4(%%esp), %%eax\n"
    "orb $12, %%ah\n"
    "movw %%ax, (%%esp)\n"
    "fldcw (%%esp)\n"
    "frndint\n"
    "fldcw 4(%%esp)\n"
    "addl $8, %%esp\n\tpop %%eax\n" : "=t" (int_part) : "0" (value) : "eax"); /* round */
#else
  int_part = trunc(value);
#endif
  if (iptr)
    *iptr = int_part;
  return (nocrt_isinf(value) ?  0.0 : value - int_part);
}

#endif
