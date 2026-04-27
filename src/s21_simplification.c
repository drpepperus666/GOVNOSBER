#include "s21_internal.h"

int s21_simplification(unsigned long long* overflow, int* scale_1,
                       s21_decimal* res, int znak) {
  int error = 0;
  for (; *overflow > 0 && *scale_1 > 0; (*scale_1)--) {
    unsigned long long rem = 0;
    for (int i = 2; i >= 0; i--) {
      unsigned long long current = (rem << 32) | res->bits[i];
      res->bits[i] = (unsigned int)(current / 10);
      rem = current % 10;
    }
    if (rem >= 5) {
      unsigned long long r = 1;
      for (int i = 0; i < 3 && r; i++) {
        r += res->bits[i];
        res->bits[i] = (unsigned int)r;
        r >>= 32;
      }
    }
    if (res->bits[2] >> 31)
      *overflow = 1;
    else
      *overflow = 0;
  }
  if (*overflow > 0) error = znak ? 2 : 1;

  return error;
}
