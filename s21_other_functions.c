#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal* result) {
  int error = 0;
  if (!result) {
    error = 1;
  } else {
    *result = value;

    for (int scale = (value.bits[3] >> 16) & 0xFF; scale > 0; scale--) {
      unsigned long long rem = 0;
      for (int i = 2; i >= 0; i--) {
        unsigned long long temp = (rem << 32) | result->bits[i];
        result->bits[i] = (unsigned int)(temp / 10);
        rem = temp % 10;
      }
    }
    if (1) {
      if (result->bits[0] == 0 && result->bits[1] == 0 &&
          result->bits[2] == 0) {
        result->bits[3] = 0;
      } else {
        result->bits[3] &= (1u << 31);
      }
    }
  }
  return error;
}

int s21_negate(s21_decimal value, s21_decimal* result) {
  int error = 0;
  if (!result)
    error = 1;
  else {
    *result = value;
    result->bits[3] ^= (1u << 31);
  }

  return error;
}

int s21_floor(s21_decimal value, s21_decimal* result) {
  int error = 0;
  if (!result) {
    error = 1;
  } else {
    s21_decimal res = value;
    unsigned int sign = (res.bits[3] >> 31) & 1;
    int scale = (res.bits[3] >> 16) & 0xFF;
    int has_fraction = 0;

    while (scale > 0) {
      unsigned long long rem = 0;
      for (int i = 2; i >= 0; i--) {
        unsigned long long temp = (rem << 32) | res.bits[i];
        res.bits[i] = (unsigned int)(temp / 10);
        rem = temp % 10;
      }
      if (rem > 0) has_fraction = 1;
      scale--;
    }

    if (res.bits[0] == 0 && res.bits[1] == 0 && res.bits[2] == 0 &&
        !has_fraction) {
      sign = 0;
    }

    res.bits[3] = (sign << 31);

    if (sign && has_fraction) {
      s21_decimal one = {{1, 0, 0, 0}};
      error = s21_sub(res, one, result);
    } else {
      *result = res;
    }
  }
  return error;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  int error = 0;
  if (!result) {
    error = 1;
  } else {
    s21_decimal res = value;
    unsigned int sign = (res.bits[3] >> 31) & 1;
    int scale = (res.bits[3] >> 16) & 0xFF;
    unsigned long long last_rem = 0;
    int has_tail = 0;

    while (scale > 0) {
      unsigned long long rem = 0;
      for (int i = 2; i >= 0; i--) {
        unsigned long long temp = (rem << 32) | res.bits[i];
        res.bits[i] = (unsigned int)(temp / 10);
        rem = temp % 10;
      }
      if (scale > 1 && last_rem > 0) has_tail = 1;
      last_rem = rem;
      scale--;
    }
    res.bits[3] = (sign << 31);

    if (last_rem > 5 || (last_rem == 5 && (has_tail || (res.bits[0] & 1)))) {
      s21_decimal one = {{1, 0, 0, 0}};
      if (sign) {
        error = s21_sub(res, one, result);
      } else {
        error = s21_add(res, one, result);
      }
    } else {
      if (res.bits[0] == 0 && res.bits[1] == 0 && res.bits[2] == 0) {
        res.bits[3] = 0;
      }
      *result = res;
    }
  }
  return error;
}