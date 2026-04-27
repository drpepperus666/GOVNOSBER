#include "s21_internal.h"

void s21_shift(s21_decimal* rem) {
  unsigned long long over = 0;
  for (int i = 2; i >= 0; i--) {
    unsigned long long shift = ((unsigned long long)rem->bits[i] << 1) | over;
    rem->bits[i] = (unsigned int)shift;
    over = shift >> 32;
  }
}
