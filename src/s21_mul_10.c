#include "s21_internal.h"

void s21_mul_10(s21_decimal* min_scale, unsigned long long* overflow) {
  *overflow = 0;
  for (int i = 0; i < 3; i++) {
    *overflow += (unsigned long long)min_scale->bits[i] * 10;
    min_scale->bits[i] = (unsigned int)*overflow;
    *overflow >>= 32;
  }
}
