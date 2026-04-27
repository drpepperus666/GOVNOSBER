#include <math.h>

#include "test_common.h"

START_TEST(from_float_null_dst) {
  ck_assert_int_eq(s21_from_float_to_decimal(1.0f, NULL), 1);
}
END_TEST

START_TEST(from_float_nan) {
  s21_decimal d = {{7, 7, 7, 7}};
  ck_assert_int_eq(s21_from_float_to_decimal(NAN, &d), 1);
}
END_TEST

START_TEST(from_float_pos_inf) {
  s21_decimal d = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_float_to_decimal(INFINITY, &d), 1);
}
END_TEST

START_TEST(from_float_neg_inf) {
  s21_decimal d = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_float_to_decimal(-INFINITY, &d), 1);
}
END_TEST

START_TEST(from_float_integer_scale_0) {
  s21_decimal d = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_float_to_decimal(123.0f, &d), 0);
  ck_assert_decimal_eq(d, s21_dec_make(123, 0, 0, 0, 0));
}
END_TEST

START_TEST(from_float_negative_integer) {
  s21_decimal d = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_float_to_decimal(-42.0f, &d), 0);
  ck_assert_decimal_eq(d, s21_dec_make(42, 0, 0, 0, 1));
}
END_TEST

START_TEST(from_float_half_scale_1) {
  s21_decimal d = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_float_to_decimal(0.5f, &d), 0);
  ck_assert_decimal_eq(d, s21_dec_make(5, 0, 0, 1, 0));
}
END_TEST

START_TEST(from_float_rounding_to_int) {
  s21_decimal d = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_float_to_decimal(1.2345678f, &d), 0);
  ck_assert_int_eq((int)((d.bits[3] >> 31) & 1u), 0);
  ck_assert_int_eq((int)((d.bits[3] >> 16) & 0xFFu) > 0, 1);
  ck_assert_int_eq(d.bits[1], 0u);
  ck_assert_int_eq(d.bits[2], 0u);
}
END_TEST

START_TEST(from_float_7_sig_digits_bankers_even_goes_down) {
  // Spec: if > 7 significant decimal digits -> bankers rounding to 7 digits.
  // 1234566.5 => "12345665" (8 sig digits), tie at last digit 5,
  // previous digit is even (6) => round DOWN => 1234566.
  s21_decimal d = {{0, 0, 0, 0}};
  int err = s21_from_float_to_decimal(1234566.5f, &d);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(d, s21_dec_make(1234566u, 0, 0, 0, 0));
}
END_TEST

START_TEST(from_float_7_sig_digits_bankers_odd_goes_up) {
  // 1234567.5 => previous digit odd (7) => round UP => 1234568.
  s21_decimal d = {{0, 0, 0, 0}};
  int err = s21_from_float_to_decimal(1234567.5f, &d);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(d, s21_dec_make(1234568u, 0, 0, 0, 0));
}
END_TEST

START_TEST(from_float_7_sig_digits_bankers_negative) {
  // Same rule for negative.
  s21_decimal d = {{0, 0, 0, 0}};
  int err = s21_from_float_to_decimal(-1234567.5f, &d);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(d, s21_dec_make(1234568u, 0, 0, 0, 1));
}
END_TEST

START_TEST(from_float_tiny_fraction_becomes_zero) {
  s21_decimal d = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_float_to_decimal(1e-8f, &d), 0);
  ck_assert_decimal_eq(d, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(from_float_too_small_error) {
  s21_decimal d = {{1, 1, 1, 1}};
  int err = s21_from_float_to_decimal(1e-29f, &d);
  ck_assert_int_eq(err, 1);
  ck_assert_decimal_eq(d, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(from_float_too_big_error) {
  s21_decimal d = {{1, 1, 1, 1}};
  int err = s21_from_float_to_decimal(1e30f, &d);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(from_float_zero_is_zero) {
  s21_decimal d = {{1, 2, 3, 4}};
  ck_assert_int_eq(s21_from_float_to_decimal(0.0f, &d), 0);
  ck_assert_decimal_eq(d, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(from_float_negative_zero_is_zero) {
  s21_decimal d = {{1, 2, 3, 4}};
  ck_assert_int_eq(s21_from_float_to_decimal(-0.0f, &d), 0);
  ck_assert_decimal_eq(d, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(to_float_null_dst) {
  s21_decimal a = s21_dec_make(1, 0, 0, 0, 0);
  ck_assert_int_eq(s21_from_decimal_to_float(a, NULL), 1);
}
END_TEST

START_TEST(to_float_scale_too_big) {
  s21_decimal a = s21_dec_make(1, 0, 0, 29, 0);
  float out = 123.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(a, &out), 1);
}
END_TEST

START_TEST(to_float_basic_positive) {
  s21_decimal a = s21_dec_make(123, 0, 0, 0, 0);
  float out = 0.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(a, &out), 0);
  ck_assert_float_eq_tol(out, 123.0f, 1e-6f);
}
END_TEST

START_TEST(to_float_basic_negative_with_scale) {
  s21_decimal a = s21_dec_make(1234, 0, 0, 2, 1);  // -12.34
  float out = 0.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(a, &out), 0);
  ck_assert_float_eq_tol(out, -12.34f, 1e-4f);
}
END_TEST

START_TEST(to_float_overflow_error) {
  s21_decimal a = s21_dec_make(0, 0, 0xFFFFFFFFu, 0, 0);
  float out = 0.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(a, &out), 1);
}
END_TEST

Suite *s21_float_converters_suite(void) {
  Suite *s = suite_create("float_converters");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, from_float_null_dst);
  tcase_add_test(tc, from_float_nan);
  tcase_add_test(tc, from_float_pos_inf);
  tcase_add_test(tc, from_float_neg_inf);
  tcase_add_test(tc, from_float_integer_scale_0);
  tcase_add_test(tc, from_float_negative_integer);
  tcase_add_test(tc, from_float_half_scale_1);
  tcase_add_test(tc, from_float_rounding_to_int);
  tcase_add_test(tc, from_float_7_sig_digits_bankers_even_goes_down);
  tcase_add_test(tc, from_float_7_sig_digits_bankers_odd_goes_up);
  tcase_add_test(tc, from_float_7_sig_digits_bankers_negative);
  tcase_add_test(tc, from_float_tiny_fraction_becomes_zero);
  tcase_add_test(tc, from_float_too_small_error);
  tcase_add_test(tc, from_float_too_big_error);
  tcase_add_test(tc, from_float_zero_is_zero);
  tcase_add_test(tc, from_float_negative_zero_is_zero);
  tcase_add_test(tc, to_float_null_dst);
  tcase_add_test(tc, to_float_scale_too_big);
  tcase_add_test(tc, to_float_basic_positive);
  tcase_add_test(tc, to_float_basic_negative_with_scale);
  tcase_add_test(tc, to_float_overflow_error);

  suite_add_tcase(s, tc);
  return s;
}
