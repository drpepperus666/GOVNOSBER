#include "test_common.h"

static s21_decimal s21_dec_max(int scale, int sign) {
  return s21_dec_make(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, scale, sign);
}

START_TEST(add_alignment_scale_28) {
  s21_decimal a = s21_dec_make(10, 0, 0, 1, 0);  // 1.0
  s21_decimal b = s21_dec_make(1, 0, 0, 28, 0);  // 1e-28
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);

  // 1.0 aligned to scale 28 => 10^28, plus 1 => 10^28 + 1
  s21_decimal expected =
      s21_dec_make(0x10000001u, 0x3E250261u, 0x204FCE5Eu, 28, 0);
  ck_assert_decimal_bits_eq(res, expected);
}
END_TEST

START_TEST(add_alignment_scale_28_negative) {
  s21_decimal a = s21_dec_make(10, 0, 0, 1, 1);  // -1.0
  s21_decimal b = s21_dec_make(1, 0, 0, 28, 0);  // +1e-28
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);

  // -1.0 + 1e-28 = -(10^28 - 1) at scale 28
  // 10^28 - 1 = 9999999999999999999999999999
  s21_decimal expected =
      s21_dec_make(0x0FFFFFFFu, 0x3E250261u, 0x204FCE5Eu, 28, 1);
  ck_assert_decimal_bits_eq(res, expected);
}
END_TEST

START_TEST(add_overflow_during_normalization_error_1) {
  s21_decimal max = s21_dec_max(0, 0);
  s21_decimal b = s21_dec_make(1, 0, 0, 1, 0);  // 0.1
  s21_decimal res = {{0, 0, 0, 0}};

  // scale_1 < scale_2 => normalization tries to multiply MAX by 10 => overflow.
  int err = s21_add(max, b, &res);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(add_overflow_during_normalization_error_2) {
  s21_decimal min = s21_dec_max(0, 1);          // -MAX
  s21_decimal b = s21_dec_make(1, 0, 0, 1, 0);  // +0.1
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(min, b, &res);
  ck_assert_int_eq(err, 2);
}
END_TEST

START_TEST(add_signs_different_10_plus_neg_5) {
  s21_decimal a = s21_dec_make(10, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(5, 0, 0, 0, 1);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(5, 0, 0, 0, 0));
}
END_TEST

START_TEST(add_signs_different_neg_10_plus_5) {
  s21_decimal a = s21_dec_make(10, 0, 0, 0, 1);
  s21_decimal b = s21_dec_make(5, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(5, 0, 0, 0, 1));
}
END_TEST

START_TEST(add_both_negative) {
  s21_decimal a = s21_dec_make(10, 0, 0, 0, 1);
  s21_decimal b = s21_dec_make(10, 0, 0, 0, 1);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(20, 0, 0, 0, 1));
}
END_TEST

START_TEST(sub_cross_zero_5_minus_10_is_neg_5) {
  s21_decimal a = s21_dec_make(5, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(10, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_sub(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(5, 0, 0, 0, 1));
}
END_TEST

START_TEST(sub_borrow_across_32bit_word) {
  s21_decimal a = s21_dec_make(0, 1, 0, 0, 0);  // 2^32
  s21_decimal b = s21_dec_make(1, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_sub(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_bits_eq(res, s21_dec_make(0xFFFFFFFFu, 0, 0, 0, 0));
}
END_TEST

START_TEST(add_carry_across_32bit_word) {
  s21_decimal a = s21_dec_make(0xFFFFFFFFu, 0, 0, 0, 0);  // 2^32 - 1
  s21_decimal b = s21_dec_make(1, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_bits_eq(res, s21_dec_make(0, 1, 0, 0, 0));
}
END_TEST

START_TEST(add_scale_equal_keeps_scale) {
  s21_decimal a = s21_dec_make(123, 0, 0, 2, 0);  // 1.23
  s21_decimal b = s21_dec_make(234, 0, 0, 2, 0);  // 2.34
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(357, 0, 0, 2, 0));
}
END_TEST

START_TEST(add_max_plus_one_error_1) {
  s21_decimal max = s21_dec_max(0, 0);
  s21_decimal one = s21_dec_make(1, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(max, one, &res);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(sub_min_minus_one_error_2) {
  s21_decimal min = s21_dec_max(0, 1);  // -MAX
  s21_decimal one = s21_dec_make(1, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_sub(min, one, &res);
  ck_assert_int_eq(err, 2);
}
END_TEST

Suite *s21_arithmetic_basic_suite(void) {
  Suite *s = suite_create("add_sub_boundaries");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, add_alignment_scale_28);
  tcase_add_test(tc, add_alignment_scale_28_negative);
  tcase_add_test(tc, add_overflow_during_normalization_error_1);
  tcase_add_test(tc, add_overflow_during_normalization_error_2);
  tcase_add_test(tc, add_signs_different_10_plus_neg_5);
  tcase_add_test(tc, add_signs_different_neg_10_plus_5);
  tcase_add_test(tc, add_both_negative);
  tcase_add_test(tc, sub_cross_zero_5_minus_10_is_neg_5);
  tcase_add_test(tc, sub_borrow_across_32bit_word);
  tcase_add_test(tc, add_carry_across_32bit_word);
  tcase_add_test(tc, add_scale_equal_keeps_scale);
  tcase_add_test(tc, add_max_plus_one_error_1);
  tcase_add_test(tc, sub_min_minus_one_error_2);

  suite_add_tcase(s, tc);
  return s;
}
