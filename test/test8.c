#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "nocrt.h"

int main()
{
  printf("SIN (double): %lf %lf %lf %lf %lf\n", sin(0), sin(1), sin(NOCRT_PI), sin(NOCRT_PI/2), sin(NOCRT_PI/3));
  printf("SIN  (float): %f %f %f %f %f\n", sinf(0), sinf(1), sinf(NOCRT_PI), sinf(NOCRT_PI/2), sinf(NOCRT_PI/3));
  printf("SIN (taylor): %lf %lf %lf %lf %lf\n",
  	nocrt_sin_eps(0,           NOCRT_DOUBLE_MAX_DIGITS),
  	nocrt_sin_eps(1,           NOCRT_DOUBLE_MAX_DIGITS),
  	nocrt_sin_eps(NOCRT_PI,    NOCRT_DOUBLE_MAX_DIGITS),
  	nocrt_sin_eps((NOCRT_PI/2), NOCRT_DOUBLE_MAX_DIGITS),
  	nocrt_sin_eps((NOCRT_PI/3), NOCRT_DOUBLE_MAX_DIGITS)
  );

  printf("COS (double): %lf %lf %lf %lf %lf\n", cos(0), sin(1), cos(NOCRT_PI), cos(NOCRT_PI/2), cos(NOCRT_PI/3));
  printf("COS  (float): %f %f %f %f %f\n", cosf(0), cosf(1), cosf(NOCRT_PI), cosf(NOCRT_PI/2), cosf(NOCRT_PI/3));
  printf("COS (taylor): %lf %lf %lf %lf %lf\n",
  	nocrt_cos_eps(0,           NOCRT_DOUBLE_MAX_DIGITS),
  	nocrt_cos_eps(1,           NOCRT_DOUBLE_MAX_DIGITS),
  	nocrt_cos_eps(NOCRT_PI,    NOCRT_DOUBLE_MAX_DIGITS),
  	nocrt_cos_eps((NOCRT_PI/2), NOCRT_DOUBLE_MAX_DIGITS),
  	nocrt_cos_eps((NOCRT_PI/3), NOCRT_DOUBLE_MAX_DIGITS)
  );

  printf("TAN (double): %lf %lf %lf %lf %lf\n", tan(0), tan(1), tan(NOCRT_PI), tan(NOCRT_PI/2), tan(NOCRT_PI/3));
  printf("TAN  (float): %f %f %f %f %f\n", tanf(0), tanf(1), tanf(NOCRT_PI), tanf(NOCRT_PI/2), tanf(NOCRT_PI/3));
  printf("TAN (taylor): %lf %lf %lf %lf %lf\n",
  	nocrt_tan_eps(0, NOCRT_DOUBLE_MAX_DIGITS),
  	nocrt_tan_eps(1, NOCRT_DOUBLE_MAX_DIGITS),
  	nocrt_tan_eps(NOCRT_PI, NOCRT_DOUBLE_MAX_DIGITS),
  	nocrt_tan_eps(NOCRT_PI/2, NOCRT_DOUBLE_MAX_DIGITS),
  	nocrt_tan_eps(NOCRT_PI/3, NOCRT_DOUBLE_MAX_DIGITS)
  );

  return EXIT_SUCCESS;
}
