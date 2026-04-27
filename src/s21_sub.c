#include "s21_internal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (value_2.bits[3] & 0x80000000)
    value_2.bits[3] &= 0x7FFFFFFF;
  else
    value_2.bits[3] |= 0x80000000;

  return s21_add(value_1, value_2, result);
}
