#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  int error = 0;
  if (!dst) {
    error = 1;
  } else {
    *dst = (s21_decimal){0};
    if (src < 0) {
      dst->bits[3] |= (1u << 31);
      dst->bits[0] = (unsigned int)(-src);
    } else {
      dst->bits[0] = (unsigned int)src;
    }
  }
  return error;
}
