#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"

// Helper function to create s21_decimal
static inline s21_decimal s21_dec_make(unsigned int bit0, unsigned int bit1,
                                       unsigned int bit2, unsigned int scale,
                                       unsigned int sign) {
  s21_decimal d = {{bit0, bit1, bit2, (scale << 16) | (sign << 31)}};
  return d;
}

// Custom assertion macro for decimal comparison
#define ck_assert_decimal_eq(actual, expected)                           \
  do {                                                                   \
    s21_decimal _actual = (actual);                                      \
    s21_decimal _expected = (expected);                                  \
    ck_assert_msg(_actual.bits[0] == _expected.bits[0] &&                \
                      _actual.bits[1] == _expected.bits[1] &&            \
                      _actual.bits[2] == _expected.bits[2] &&            \
                      _actual.bits[3] == _expected.bits[3],              \
                  "Decimals are not equal: actual=0x%x 0x%x 0x%x 0x%x, " \
                  "expected=0x%x 0x%x 0x%x 0x%x",                        \
                  _actual.bits[0], _actual.bits[1], _actual.bits[2],     \
                  _actual.bits[3], _expected.bits[0], _expected.bits[1], \
                  _expected.bits[2], _expected.bits[3]);                 \
  } while (0)

// Alias for decimal comparison
#define ck_assert_decimal_bits_eq(actual, expected) \
  ck_assert_decimal_eq(actual, expected)

#endif
