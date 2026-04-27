#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0;
  if (!dst || isnan(src) || isinf(src)) error = 1;

  if (error == 0) *dst = (s21_decimal){0};

  int sign = (src < 0.0f) ? 1 : 0;
  if (sign) src = -src;

  int scale = 0;
  double value = (double)src;

  while (scale < 28 && value - floor(value) > 1e-7 && error == 0) {
    value *= 10.0;
    scale++;
  }
  if (error == 0) {
    unsigned long long mantissa = (unsigned long long)round(value);

    dst->bits[0] = (unsigned int)(mantissa & 0xFFFFFFFF);
    dst->bits[1] = (unsigned int)((mantissa >> 32) & 0xFFFFFFFF);
    dst->bits[2] = 0;
    dst->bits[3] = (sign << 31) | (scale << 16);
  }

  return error;
}

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