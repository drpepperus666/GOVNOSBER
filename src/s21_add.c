#include "s21_internal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int error = 0;
  if (!result) return 4;
  *result = (s21_decimal){0};

  int znak_1, znak_2, scale_1, scale_2;
  s21_znak_scale(&znak_1, &znak_2, &scale_1, &scale_2, value_1, value_2);

  if (scale_1 < scale_2)
    error = s21_normalization(&value_1, &scale_1, scale_2, znak_1);
  if (scale_2 < scale_1)
    error = s21_normalization(&value_2, &scale_2, scale_1, znak_2);

  unsigned long long overflow = 0;
  int znak = znak_1;

  if (znak_1 == znak_2) s21_plus(&overflow, result, value_1, value_2);
  for (int i = 2; (i >= 0 && (znak_1 != znak_2)); i--) {
    if (value_1.bits[i] > value_2.bits[i]) {
      s21_minus(value_2, value_1, result);
      i = -1;
    } else if (value_1.bits[i] < value_2.bits[i]) {
      s21_minus(value_1, value_2, result);
      znak = znak_2;
      i = -1;
    }
  }

  if (overflow > 0)
    error = s21_simplification(&overflow, &scale_1, result, znak);

  if (result->bits[0] == 0 && result->bits[1] == 0 && result->bits[2] == 0)
    znak = 0;
  result->bits[3] = (scale_1 << 16) & 0x00FF0000;
  if (znak == 1) result->bits[3] |= 0x80000000;

  return error;
}
