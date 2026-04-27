#include "s21_internal.h"

void s21_plus(unsigned long long* overflow, s21_decimal* res, s21_decimal val_1,
              s21_decimal val_2) {
  *overflow = 0;
  for (int i = 0; i < 3; i++) {
    *overflow += (unsigned long long)val_1.bits[i] + val_2.bits[i];
    res->bits[i] = (unsigned int)*overflow;
    *overflow >>= 32;
  }
}
