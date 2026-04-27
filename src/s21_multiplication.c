#include "s21_internal.h"

void s21_multiplication(s21_decimal value_1, s21_decimal value_2,
                        s21_decimal* result, unsigned long long* overflow) {
  unsigned long long temp[6] = {0};

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      unsigned long long mul =
          (unsigned long long)value_1.bits[i] * value_2.bits[j];
      temp[i + j] += mul & 0xFFFFFFFF;
      temp[i + j + 1] += mul >> 32;

      for (int k = i + j; temp[k] > 0xFFFFFFFF; k++) {
        temp[k + 1] += temp[k] >> 32;
        temp[k] &= 0xFFFFFFFF;
      }
    }
  }

  result->bits[0] = (unsigned int)temp[0];
  result->bits[1] = (unsigned int)temp[1];
  result->bits[2] = (unsigned int)temp[2];

  *overflow = (temp[3] > 0 || temp[4] > 0 || temp[5] > 0) ? 1 : 0;
}
