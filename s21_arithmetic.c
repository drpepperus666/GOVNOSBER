#include "s21_decimal.h"

void s21_znak_scale(int *znak_1, int *znak_2, int *scale_1, int *scale_2,
                    s21_decimal value_1, s21_decimal value_2) {
  *znak_1 = (value_1.bits[3] & 0x80000000) ? 1 : 0;
  *znak_2 = (value_2.bits[3] & 0x80000000) ? 1 : 0;

  *scale_1 = (value_1.bits[3] >> 16) & 0xFF;
  *scale_2 = (value_2.bits[3] >> 16) & 0xFF;
}

void s21_mul_10(s21_decimal *min_scale, unsigned long long *overflow) {
  *overflow = 0;
  for (int i = 0; i < 3; i++) {
    *overflow += (unsigned long long)min_scale->bits[i] * 10;
    min_scale->bits[i] = (unsigned int)*overflow;
    *overflow >>= 32;
  }
}

int s21_normalization(s21_decimal *min_scale, int *scale_1, int scale_2,
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

void s21_minus(s21_decimal min_value, s21_decimal max_value, s21_decimal *res) {
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

void s21_plus(unsigned long long *overflow, s21_decimal *res, s21_decimal val_1,
              s21_decimal val_2) {
  *overflow = 0;
  for (int i = 0; i < 3; i++) {
    *overflow += (unsigned long long)val_1.bits[i] + val_2.bits[i];
    res->bits[i] = (unsigned int)*overflow;
    *overflow >>= 32;
  }
}

int s21_simplification(unsigned long long *overflow, int *scale_1,
                       s21_decimal *res, int znak) {
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

void s21_multiplication(s21_decimal value_1, s21_decimal value_2,
                        s21_decimal *result, unsigned long long *overflow) {
  unsigned long long temp[6] = {0};

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      unsigned long long mul =
          (unsigned long long)value_1.bits[i] * value_2.bits[j];
      temp[i + j] += mul & 0xFFFFFFFF;
      temp[i + j + 1] += mul >> 32;

      for (int k = i + j; temp[k] > 0xFFFFFFFF; k++) {
        temp[k + 1] += temp[k] >> 32;
        temp[k] &= 0xFFFFFFFF;
      }
    }
  }

  result->bits[0] = (unsigned int)temp[0];
  result->bits[1] = (unsigned int)temp[1];
  result->bits[2] = (unsigned int)temp[2];

  *overflow = (temp[3] > 0 || temp[4] > 0 || temp[5] > 0) ? 1 : 0;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  if (!result) error = 4;
  *result = (s21_decimal){0};

  int znak_1, znak_2, scale_1, scale_2;
  s21_znak_scale(&znak_1, &znak_2, &scale_1, &scale_2, value_1, value_2);

  if (scale_1 < scale_2)
    error = s21_normalization(&value_1, &scale_1, scale_2, znak_1);
  if (scale_2 < scale_1)
    error = s21_normalization(&value_2, &scale_2, scale_1, znak_2);

  unsigned long long overflow = 0;
  int znak = znak_1;

  if (znak_1 == znak_2) s21_plus(&overflow, result, value_1, value_2);
  for (int i = 2; (i >= 0 && (znak_1 != znak_2)); i--) {
    if (value_1.bits[i] > value_2.bits[i]) {
      s21_minus(value_2, value_1, result);
      i = -1;
    } else if (value_1.bits[i] < value_2.bits[i]) {
      s21_minus(value_1, value_2, result);
      znak = znak_2;
      i = -1;
    }
  }

  if (overflow > 0)
    error = s21_simplification(&overflow, &scale_1, result, znak);

  if (result->bits[0] == 0 && result->bits[1] == 0 && result->bits[2] == 0)
    znak = 0;
  result->bits[3] = (scale_1 << 16) & 0x00FF0000;
  if (znak == 1) result->bits[3] |= 0x80000000;

  return error;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (value_2.bits[3] & 0x80000000)
    value_2.bits[3] &= 0x7FFFFFFF;
  else
    value_2.bits[3] |= 0x80000000;

  return s21_add(value_1, value_2, result);
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  if (!result) return 4;
  *result = (s21_decimal){0};

  int znak_1, znak_2, scale_1, scale_2;
  s21_znak_scale(&znak_1, &znak_2, &scale_1, &scale_2, value_1, value_2);

  int znak = znak_1 ^ znak_2;
  int scale = scale_1 + scale_2;

  int is_zero = 0;

  if ((value_1.bits[0] == 0 && value_1.bits[1] == 0 && value_1.bits[2] == 0) ||
      (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0)) {
    is_zero = 1;
    znak = 0;
    scale = 0;
  }

  unsigned long long overflow = 0;
  if (is_zero == 0) s21_multiplication(value_1, value_2, result, &overflow);

  if (overflow > 0 || scale > 28)
    error = s21_simplification(&overflow, &scale, result, znak);

  result->bits[3] = (scale << 16) & 0x00FF0000;
  if (znak == 1) result->bits[3] |= 0x80000000;

  return error;
}

void s21_shift(s21_decimal *rem) {
  unsigned long long over = 0;
  for (int i = 2; i >= 0; i--) {
    unsigned long long shift = ((unsigned long long)rem->bits[i] << 1) | over;
    rem->bits[i] = (unsigned int)shift;
    over = shift >> 32;
  }
}

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

void s21_del_int(s21_decimal *remainder, s21_decimal value_1,
                 s21_decimal value_2, s21_decimal *result) {
  for (int bit = 95; bit >= 0; bit--) {
    s21_shift(remainder);

    int index = bit / 32;
    int pos = bit % 32;
    if ((value_1.bits[index] >> pos) & 1) remainder->bits[0] |= 1;

    s21_shift(result);

    int can = s21_compare(*remainder, value_2);

    if (can) {
      s21_minus(value_2, *remainder, remainder);
      result->bits[0] |= 1;
    }
  }
}

int s21_del_fraction(s21_decimal *remainder, s21_decimal value_2,
                     s21_decimal *result, int znak, int *scale) {
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

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  if (!result)
    error = 4;
  else
    *result = (s21_decimal){0};

  int znak_1, znak_2, scale_1, scale_2;
  s21_znak_scale(&znak_1, &znak_2, &scale_1, &scale_2, value_1, value_2);
  int znak = znak_1 ^ znak_2;
  int scale = scale_1 - scale_2;

  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0)
    error = 3;

  if (scale < 0 && error == 0) {
    error = s21_normalization(&value_1, &scale_1, (scale_1 - scale), znak_1);
    scale = 0;
  }

  s21_decimal remainder = {0};
  if (error == 0) {
    s21_del_int(&remainder, value_1, value_2, result);
    error = s21_del_fraction(&remainder, value_2, result, znak, &scale);
  }

  if (result->bits[0] == 0 && result->bits[1] == 0 && result->bits[2] == 0) {
    znak = 0;
    scale = 0;
  }

  if (error == 0) result->bits[3] = (scale << 16) & 0x00FF0000;
  if (znak == 1 && error == 0) result->bits[3] |= 0x80000000;

  return error;
}
