/******************************************************************************\
* MIT No Attribution                                                           *
*                                                                              *
* Copyright (c) 2010-2024 Jaroslav Hensl                                       *
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

#include <math.h>

#include "nocrt.h"

/* JH 2024: this all is pretty naive, but also fast way how make code portable,
 *          but very ineffective!
 */

typedef union _editable_double_t
{
	double number;
	unsigned char direct[sizeof(double)];
} editable_double_t;

#ifdef __BIG_ENDIAN__
# define ED_FIRST_BYTE  0
# define ED_SECOND_BYTE 1
#else
# define ED_FIRST_BYTE  7
# define ED_SECOND_BYTE 6
#endif


#define P2_DEF_EXP 0x3FF              /* exponent of number 1.0 */

#define REM_SING(_ed) _ed.direct[ED_FIRST_BYTE] &= 0x7F

#define EXP_GET(_ed) (((_ed.direct[ED_FIRST_BYTE]&0x7F) << 4) | \
	(_ed.direct[ED_SECOND_BYTE] >> 4))

#define EXP_SET(_ed, exp)  _ed.direct[ED_FIRST_BYTE] &= 0x80; \
	_ed.direct[ED_FIRST_BYTE] |= (exp >> 4) & 0x7F; \
	_ed.direct[ED_SECOND_BYTE] &= 0x0F; _ed.direct[ED_SECOND_BYTE] |= exp << 4;

static double eps_tables[NOCRT_DOUBLE_MAX_DIGITS] =
{
	1,
	0.1d,
	0.01d,
	0.001d,
	0.0001d,
	0.00001d,
	0.000001d,
	0.0000001d,
	0.00000001d,
	0.000000001d,
	0.0000000001d,
	0.00000000001d,
	0.000000000001d,
	0.0000000000001d,
	0.00000000000001d,
	0.000000000000001d
};

static double nocrt_eps(unsigned int digits)
{
	if(digits >= NOCRT_DOUBLE_MAX_DIGITS)
		digits = NOCRT_DOUBLE_MAX_DIGITS-1;

	return eps_tables[digits];
}

double nocrt_log_eps(double z, unsigned int digits)
{
	double from;
	unsigned int counter = 0;
	double powered;
	double rval = 0;
	double eps = nocrt_eps(digits);
	editable_double_t last, test;
	short exp;

	if(isnan(z) || z < 0 || z == INFINITY || digits <= 0)
	{
		return NAN;
	}

	if(z == 0)
	{
		return -INFINITY;
	}

	/* get a exponent */
	test.number = z;
	exp = EXP_GET(test);
	exp -= P2_DEF_EXP;
	/* set the exponent to same as have 1.0f */
	EXP_SET(test, P2_DEF_EXP);
	z = test.number;

	counter = 3; /* ve start at 3! */
	from = (z-1)/(z+1); /* the base of series */
	powered = from;
	rval = powered;
	last.number = rval;
	REM_SING(last); /* same as abs(last) */
	test.number = 0;

	while(last.number > test.number)
	{
		powered *= from * from; /* power on 3 */
		last.number = (powered/counter);
		rval += last.number;
		counter += 2; /* we moving by 2 steps like 3! 5! 7! ... */

		test.number = rval * eps;
		REM_SING(last); /* same as last = abs(last) */
		REM_SING(test); /* same as test = abs(test) */
	}

	return rval * 2 + exp * NOCRT_LN2; /* P2_LN2 is constant for fast calculation */
}

double nocrt_sin_eps(double x, int digits)
{
	editable_double_t test, last;
	double fact = 1.0f;
	double powered = x;
	double counter = 1.0f;
	double rval = x;
	double eps = nocrt_eps(digits);
	double add_sub = -1.0;

	if(x == NAN || x == INFINITY || digits <= 0)
	{
		return NAN;
	}

	last.number = x;
	test.number = x * eps;
	REM_SING(last);
	REM_SING(test);

	while(last.number > test.number)
	{
		powered *= x * x;
		fact *= (counter + 1) * (counter + 2); /* factorial calculation */
		counter += 2; /* plus 2, because ve move by 2 */
		last.number = powered/fact;
		rval += last.number * add_sub;
		test.number = rval * eps;
		add_sub = -add_sub;
		REM_SING(test); /* same as test = abs(test) */
		REM_SING(last); /* same as last = abs(last) */
	}

	return rval;
}

double nocrt_cos_eps(double x, int digits)
{
	editable_double_t test, last;
	double fact = 2.0f;
	double powered = 1.0f;
	double counter = 2.0f;
	double rval = 1.0f;
	double eps = nocrt_eps(digits);
	double add_sub = -1.0;

	if(x == NAN || x == INFINITY || digits <= 0)
	{
		return NAN;
	}

	last.number = x;
	test.number = x * eps;
	REM_SING(last);
	REM_SING(test);

	while(last.number > test.number)
	{
		powered *= x * x;
		last.number = powered/fact;
		rval += last.number * add_sub;
		test.number = rval * eps;

		fact *= (counter + 1) * (counter + 2); /* factorial calculation */
		counter += 2; /* plus 2, because ve move by 2 */
		add_sub = -add_sub;
		REM_SING(test); /* same as test = abs(test) */
		REM_SING(last); /* same as last = abs(last) */
	}

	return rval;
}

double nocrt_tan_eps(double x, int digits)
{
	double eps = nocrt_eps(digits);
	double a = nocrt_sin_eps(x, digits);	
	double b = nocrt_cos_eps(x, digits);
	if(b >= -eps && b <= eps)
	{
		if(a >= 0)
		{
			return INFINITY;
		}
		else
		{
			return -INFINITY;
		}
	}

	return a/b;
}

float nocrt_logf(float x)
{
	return nocrt_log_eps(x, NOCRT_FLOAT_MAX_DIGITS);
}

/* exports */

#ifndef NOCRT_FLOAT

float nocrt_sinf(float x)
{
	return nocrt_sin_eps(x, NOCRT_FLOAT_MAX_DIGITS);
}

float nocrt_cosf(float x)
{
	return nocrt_cos_eps(x, NOCRT_FLOAT_MAX_DIGITS);
}

float nocrt_tanf(float x)
{
	return nocrt_tan_eps(x, NOCRT_FLOAT_MAX_DIGITS);
}

double nocrt_log(double x)
{
	return nocrt_log_eps(x, NOCRT_DOUBLE_MAX_DIGITS);
}

double nocrt_sin(double x)
{
	return nocrt_sin_eps(x, NOCRT_DOUBLE_MAX_DIGITS);
}

double nocrt_cos(double x)
{
	return nocrt_cos_eps(x, NOCRT_DOUBLE_MAX_DIGITS);
}

float nocrt_tan(float x)
{
	return nocrt_tan_eps(x, NOCRT_DOUBLE_MAX_DIGITS);
}

#endif
