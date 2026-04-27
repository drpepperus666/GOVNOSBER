#include "s21_internal.h"

void s21_znak_scale(int* znak_1, int* znak_2, int* scale_1, int* scale_2,
                    s21_decimal value_1, s21_decimal value_2) {
  *znak_1 = (value_1.bits[3] & 0x80000000) ? 1 : 0;
  *znak_2 = (value_2.bits[3] & 0x80000000) ? 1 : 0;

  *scale_1 = (value_1.bits[3] >> 16) & 0xFF;
  *scale_2 = (value_2.bits[3] >> 16) & 0xFF;
}
