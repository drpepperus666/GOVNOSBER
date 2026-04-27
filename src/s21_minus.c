#include "s21_internal.h"

void s21_minus(s21_decimal min_value, s21_decimal max_value, s21_decimal* res) {
  long long overflow = 0;
  for (int i = 0; i < 3; i++) {
    long long diff =
        (long long)max_value.bits[i] - min_value.bits[i] - overflow;
    if (diff < 0) {
      diff += (1LL << 32);
      overflow = 1;
    } else
      overflow = 0;
    res->bits[i] = (unsigned int)diff;
  }
}
