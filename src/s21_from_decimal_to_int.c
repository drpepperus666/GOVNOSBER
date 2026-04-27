#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  int error = 0;
  if (!dst) {
    error = 1;
  } else {
    s21_decimal temp = src;
    int sign = (temp.bits[3] >> 31) & 1;
    int scale = (temp.bits[3] >> 16) & 0xFF;

    while (scale > 0) {
      unsigned long long rem = 0;
      for (int i = 2; i >= 0; i--) {
        unsigned long long current = (rem << 32) | temp.bits[i];
        temp.bits[i] = current / 10;
        rem = current % 10;
      }
      scale--;
    }

    if (temp.bits[1] != 0 || temp.bits[2] != 0 || temp.bits[0] > 0x80000000 ||
        (temp.bits[0] == 0x80000000 && sign == 0)) {
      error = 1;
    } else {
      if (sign && temp.bits[0] == 0x80000000) {
        *dst = -2147483648;
      } else {
        *dst = (int)temp.bits[0];
        if (sign) *dst = -(*dst);
      }
    }
  }
  return error;
}
