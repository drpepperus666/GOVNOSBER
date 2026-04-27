#include <math.h>

#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int error = 0;
  if (!dst)
    error = 1;
  else
    *dst = 0.0f;

  int sign = (src.bits[3] >> 31) & 1;
  int scale = (src.bits[3] >> 16) & 0xFF;

  if (scale > 28) error = 1;

  if (error == 0) {
    unsigned long long low = src.bits[0];
    unsigned long long mid = src.bits[1];
    unsigned long long high = src.bits[2];

    double result = (double)high * pow(2.0, 64.0) +
                    (double)mid * pow(2.0, 32.0) + (double)low;
    result = result / pow(10.0, (double)scale);
    if (sign) result = -result;

    if (fabs(result) > 3.402823e38)
      error = 1;
    else
      *dst = (float)result;
  }

  return error;
}
