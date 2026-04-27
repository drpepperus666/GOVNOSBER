#include "s21_internal.h"

int s21_compare(s21_decimal remainder, s21_decimal value_2) {
  int can = 0;
  for (int i = 2; i >= 0; i--) {
    if (remainder.bits[i] > value_2.bits[i]) {
      can = 1;
      i = -1;
    } else if (remainder.bits[i] < value_2.bits[i])
      i = -1;

    if (i == 0) can = 1;
  }
  return can;
}
