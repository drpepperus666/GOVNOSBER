#include "s21_internal.h"

int s21_normalization(s21_decimal* min_scale, int* scale_1, int scale_2,
                      int znak) {
  int res = 0;

  while (*scale_1 < scale_2 && res == 0) {
    unsigned long long overflow = 0;
    s21_mul_10(min_scale, &overflow);

    if (overflow > 0)
      res = znak ? 2 : 1;
    else
      (*scale_1)++;
  }

  return res;
}
