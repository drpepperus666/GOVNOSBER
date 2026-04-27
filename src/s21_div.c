#include "s21_internal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int error = 0;
  if (!result) return 4;
  *result = (s21_decimal){0};

  int znak_1, znak_2, scale_1, scale_2;
  s21_znak_scale(&znak_1, &znak_2, &scale_1, &scale_2, value_1, value_2);
  int znak = znak_1 ^ znak_2;
  int scale = scale_1 - scale_2;

  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0)
    error = 3;

  if (scale < 0 && error == 0) {
    error = s21_normalization(&value_1, &scale_1, (scale_1 - scale), znak_1);
    scale = 0;
  }

  s21_decimal remainder = {0};
  if (error == 0) {
    s21_del_int(&remainder, value_1, value_2, result);
    error = s21_del_fraction(&remainder, value_2, result, znak, &scale);
  }

  if (result->bits[0] == 0 && result->bits[1] == 0 && result->bits[2] == 0) {
    znak = 0;
    if (scale > 28) scale = 28;
    if (scale < 0) scale = 0;
  }

  if (error == 0) result->bits[3] = (scale << 16) & 0x00FF0000;
  if (znak == 1 && error == 0) result->bits[3] |= 0x80000000;

  return error;
}
