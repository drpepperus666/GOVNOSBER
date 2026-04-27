#include <stdio.h>
#include <math.h>
#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0;
  if (!dst || isnan(src) || isinf(src)) error = 1;

  if (error == 0) *dst = (s21_decimal){0};

  int sign = (src < 0.0f) ? 1 : 0;
  if (sign) src = -src;

  int scale = 0;
  double value = (double)src;

  if (value > 0.0 && value < 1e-28) {
    if (dst) *dst = (s21_decimal){0};
    return 1;
  }
  if (value > 79228162514264337593543950335.0f) {
    if (dst) *dst = (s21_decimal){0};
    return 1;
  }

  char str[20];
  sprintf(str, "%.7g", value);
  sscanf(str, "%lf", &value);

  while (scale < 28 && fmod(value * pow(10.0, scale), 1.0) >= 1e-7 && error == 0) {
    scale++;
  }

  value *= pow(10.0, scale);

  if (error == 0) {
    unsigned long long mantissa = (unsigned long long)round(value);

    dst->bits[0] = (unsigned int)(mantissa & 0xFFFFFFFF);
    dst->bits[1] = (unsigned int)((mantissa >> 32) & 0xFFFFFFFF);
    dst->bits[2] = 0;
    dst->bits[3] = ((unsigned int)sign << 31) | (scale << 16);
  }

  return error;
}
