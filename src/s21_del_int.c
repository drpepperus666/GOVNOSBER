#include "s21_internal.h"

void s21_del_int(s21_decimal* remainder, s21_decimal value_1,
                 s21_decimal value_2, s21_decimal* result) {
  for (int bit = 95; bit >= 0; bit--) {
    s21_shift(remainder);

    int index = bit / 32;
    int pos = bit % 32;
    if ((value_1.bits[index] >> pos) & 1) remainder->bits[0] |= 1;

    s21_shift(result);

    int can = s21_compare(*remainder, value_2);

    if (can) {
      s21_minus(value_2, *remainder, remainder);
      result->bits[0] |= 1;
    }
  }
}
