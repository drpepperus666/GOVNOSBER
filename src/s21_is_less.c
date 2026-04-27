#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  int is_v1_zero =
      (value_1.bits[0] == 0 && value_1.bits[1] == 0 && value_1.bits[2] == 0);
  int is_v2_zero =
      (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0);

  if (!(is_v1_zero && is_v2_zero)) {
    int sign_1 = (value_1.bits[3] >> 31) & 1;
    int sign_2 = (value_2.bits[3] >> 31) & 1;

    if (sign_1 != sign_2) {
      if (sign_1 == 1) res = 1;
    } else {
      s21_decimal diff = {0};
      s21_sub(value_1, value_2, &diff);
      int res_sign = (diff.bits[3] >> 31) & 1;
      int is_diff_zero =
          (diff.bits[0] == 0 && diff.bits[1] == 0 && diff.bits[2] == 0);
      if (!is_diff_zero && res_sign == 1) res = 1;
    }
  }
  return res;
}
