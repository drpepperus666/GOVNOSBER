#include "test_common.h"

START_TEST(mul_by_zero_left) {
  s21_decimal a = s21_dec_make(0, 0, 0, 10, 1);
  s21_decimal b = s21_dec_make(123, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_mul(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(mul_by_zero_right) {
  s21_decimal a = s21_dec_make(123, 0, 0, 0, 1);
  s21_decimal b = s21_dec_make(0, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_mul(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(mul_sign_and_scale) {
  s21_decimal a = s21_dec_make(12, 0, 0, 1, 0);  // 1.2
  s21_decimal b = s21_dec_make(34, 0, 0, 1, 1);  // -3.4
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_mul(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(408, 0, 0, 2, 1));  // -4.08
}
END_TEST

START_TEST(mul_scale_sum_exact_2_5_times_4_is_10_0) {
  s21_decimal a = s21_dec_make(25, 0, 0, 1, 0);  // 2.5
  s21_decimal b = s21_dec_make(4, 0, 0, 0, 0);   // 4
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_mul(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_bits_eq(res, s21_dec_make(100, 0, 0, 1, 0));  // 10.0
}
END_TEST

START_TEST(mul_small_fractions_0_1_times_0_2_is_0_02) {
  s21_decimal a = s21_dec_make(1, 0, 0, 1, 0);  // 0.1
  s21_decimal b = s21_dec_make(2, 0, 0, 1, 0);  // 0.2
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_mul(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_bits_eq(res, s21_dec_make(2, 0, 0, 2, 0));  // 0.02
}
END_TEST

START_TEST(mul_overflow_positive_error_1) {
  s21_decimal max = s21_dec_make(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0, 0);
  s21_decimal two = s21_dec_make(2, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_mul(max, two, &res);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(mul_overflow_negative_error_2) {
  s21_decimal max = s21_dec_make(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0, 0);
  s21_decimal two_neg = s21_dec_make(2, 0, 0, 0, 1);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_mul(max, two_neg, &res);
  ck_assert_int_eq(err, 2);
}
END_TEST

START_TEST(mul_overflow_can_be_simplified) {
  s21_decimal a = s21_dec_make(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 1, 0);
  s21_decimal b = s21_dec_make(10, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_mul(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq((int)((res.bits[3] >> 16) & 0xFFu) <= 1, 1);
}
END_TEST

START_TEST(div_by_zero_error_3) {
  s21_decimal a = s21_dec_make(1, 0, 0, 0, 0);
  s21_decimal z = s21_dec_make(0, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_div(a, z, &res);
  ck_assert_int_eq(err, 3);
}
END_TEST

START_TEST(div_zero_by_number_is_zero) {
  s21_decimal z = s21_dec_make(0, 0, 0, 0, 1);
  s21_decimal b = s21_dec_make(7, 0, 0, 0, 0);
  s21_decimal res = {{1, 1, 1, 0xFFFFFFFFu}};

  int err = s21_div(z, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(div_simple_integer) {
  s21_decimal a = s21_dec_make(10, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(2, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_div(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(5, 0, 0, 0, 0));
}
END_TEST

START_TEST(div_periodic_fraction_scale_28) {
  s21_decimal a = s21_dec_make(1, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(3, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_div(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq((int)((res.bits[3] >> 16) & 0xFFu), 28);
  ck_assert_int_eq((res.bits[0] | res.bits[1] | res.bits[2]) != 0u, 1);
}
END_TEST

START_TEST(div_very_small_by_very_big_non_zero_scale_28) {
  s21_decimal small = s21_dec_make(1, 0, 0, 28, 0);  // 1e-28
  s21_decimal big = s21_dec_make(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_div(small, big, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq((int)((res.bits[3] >> 16) & 0xFFu), 28);
}
END_TEST

START_TEST(div_negative_sign) {
  s21_decimal a = s21_dec_make(10, 0, 0, 0, 1);
  s21_decimal b = s21_dec_make(4, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_div(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq((int)((res.bits[3] >> 31) & 1u), 1);
}
END_TEST

START_TEST(div_normalization_overflow_error_1) {
  s21_decimal a = s21_dec_make(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0, 0);
  s21_decimal b = s21_dec_make(1, 0, 0, 28, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  // scale = 0 - 28 => normalization will try to upscale 'a' to 28 and overflow.
  int err = s21_div(a, b, &res);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(div_normalization_overflow_error_2) {
  s21_decimal a = s21_dec_make(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0, 1);
  s21_decimal b = s21_dec_make(1, 0, 0, 28, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_div(a, b, &res);
  ck_assert_int_eq(err, 2);
}
END_TEST

START_TEST(div_fraction_10_over_4_is_2_5) {
  s21_decimal a = s21_dec_make(10, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(4, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_div(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_bits_eq(res, s21_dec_make(25, 0, 0, 1, 0));  // 2.5
}
END_TEST

START_TEST(div_fraction_1_over_8_is_0_125) {
  s21_decimal a = s21_dec_make(1, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(8, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_div(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_bits_eq(res, s21_dec_make(125, 0, 0, 3, 0));  // 0.125
}
END_TEST

START_TEST(div_negative_fraction_sign) {
  s21_decimal a = s21_dec_make(10, 0, 0, 0, 1);  // -10
  s21_decimal b = s21_dec_make(4, 0, 0, 0, 0);   // 4
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_div(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_bits_eq(res, s21_dec_make(25, 0, 0, 1, 1));  // -2.5
}
END_TEST

Suite *s21_arithmetic_complex_suite(void) {
  Suite *s = suite_create("mul_div_boundaries");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, mul_by_zero_left);
  tcase_add_test(tc, mul_by_zero_right);
  tcase_add_test(tc, mul_sign_and_scale);
  tcase_add_test(tc, mul_scale_sum_exact_2_5_times_4_is_10_0);
  tcase_add_test(tc, mul_small_fractions_0_1_times_0_2_is_0_02);
  tcase_add_test(tc, mul_overflow_positive_error_1);
  tcase_add_test(tc, mul_overflow_negative_error_2);
  tcase_add_test(tc, mul_overflow_can_be_simplified);

  tcase_add_test(tc, div_by_zero_error_3);
  tcase_add_test(tc, div_zero_by_number_is_zero);
  tcase_add_test(tc, div_simple_integer);
  tcase_add_test(tc, div_fraction_10_over_4_is_2_5);
  tcase_add_test(tc, div_fraction_1_over_8_is_0_125);
  tcase_add_test(tc, div_periodic_fraction_scale_28);
  tcase_add_test(tc, div_very_small_by_very_big_non_zero_scale_28);
  tcase_add_test(tc, div_negative_sign);
  tcase_add_test(tc, div_negative_fraction_sign);
  tcase_add_test(tc, div_normalization_overflow_error_1);
  tcase_add_test(tc, div_normalization_overflow_error_2);

  suite_add_tcase(s, tc);
  return s;
}
