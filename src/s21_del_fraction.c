#include "s21_internal.h"

int s21_del_fraction(s21_decimal* remainder, s21_decimal value_2,
                     s21_decimal* result, int znak, int* scale) {
  int error = 0;
  unsigned long long over = 0;

  while (*scale < 28 &&
         !(remainder->bits[0] == 0 && remainder->bits[1] == 0 &&
           remainder->bits[2] == 0) &&
         error == 0) {
    s21_mul_10(remainder, &over);
    s21_mul_10(result, &over);
    if (over > 0) error = znak ? 2 : 1;

    int digit = 0;
    while (s21_compare(*remainder, value_2) && error == 0) {
      s21_minus(value_2, *remainder, remainder);
      digit++;
    }

    if (digit > 0) {
      s21_decimal add_val = {{(unsigned int)digit, 0, 0, 0}};
      s21_plus(&over, result, *result, add_val);
    }

    (*scale)++;
  }

  return error;
}
