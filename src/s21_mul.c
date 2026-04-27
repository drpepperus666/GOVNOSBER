#include "s21_internal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int error = 0;
  if (!result) return 4;
  *result = (s21_decimal){0};

  int znak_1, znak_2, scale_1, scale_2;
  s21_znak_scale(&znak_1, &znak_2, &scale_1, &scale_2, value_1, value_2);

  int znak = znak_1 ^ znak_2;
  int scale = scale_1 + scale_2;

  int is_zero = 0;

  if ((value_1.bits[0] == 0 && value_1.bits[1] == 0 && value_1.bits[2] == 0) ||
      (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0)) {
    is_zero = 1;
    znak = 0;
    scale = 0;
  }

  unsigned long long overflow = 0;
  if (is_zero == 0) s21_multiplication(value_1, value_2, result, &overflow);

  if (overflow > 0 || scale > 28)
    error = s21_simplification(&overflow, &scale, result, znak);

  result->bits[3] = (scale << 16) & 0x00FF0000;
  if (znak == 1) result->bits[3] |= 0x80000000;

  return error;
}
