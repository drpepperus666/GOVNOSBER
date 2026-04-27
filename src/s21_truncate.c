#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal* result) {
  int error = 0;
  if (!result) {
    error = 1;
  } else {
    *result = value;

    for (int scale = (value.bits[3] >> 16) & 0xFF; scale > 0; scale--) {
      unsigned long long rem = 0;
      for (int i = 2; i >= 0; i--) {
        unsigned long long temp = (rem << 32) | result->bits[i];
        result->bits[i] = (unsigned int)(temp / 10);
        rem = temp % 10;
      }
    }
    if (result->bits[0] == 0 && result->bits[1] == 0 &&
        result->bits[2] == 0) {
      result->bits[3] = 0;
    } else {
      result->bits[3] &= (1u << 31);
    }
  }
  return error;
}
