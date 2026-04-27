#include "test_common.h"

START_TEST(eq_zero_pos_vs_neg) {
  s21_decimal pz = s21_dec_make(0, 0, 0, 0, 0);
  s21_decimal nz = s21_dec_make(0, 0, 0, 0, 1);

  ck_assert_int_eq(s21_is_equal(pz, nz), 1);
  ck_assert_int_eq(s21_is_not_equal(pz, nz), 0);
  ck_assert_int_eq(s21_is_less(pz, nz), 0);
  ck_assert_int_eq(s21_is_greater(pz, nz), 0);
  ck_assert_int_eq(s21_is_less_or_equal(pz, nz), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(pz, nz), 1);
}
END_TEST

START_TEST(eq_scale_ignored_1_0_vs_1_00) {
  s21_decimal a = s21_dec_make(10, 0, 0, 1, 0);   // 1.0
  s21_decimal b = s21_dec_make(100, 0, 0, 2, 0);  // 1.00

  ck_assert_int_eq(s21_is_equal(a, b), 1);
  ck_assert_int_eq(s21_is_not_equal(a, b), 0);
  ck_assert_int_eq(s21_is_less(a, b), 0);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(compare_same_value_different_scales_negative) {
  s21_decimal a = s21_dec_make(10, 0, 0, 1, 1);   // -1.0
  s21_decimal b = s21_dec_make(100, 0, 0, 2, 1);  // -1.00

  ck_assert_int_eq(s21_is_equal(a, b), 1);
  ck_assert_int_eq(s21_is_not_equal(a, b), 0);
  ck_assert_int_eq(s21_is_less(a, b), 0);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(less_with_scale_alignment) {
  s21_decimal a = s21_dec_make(9, 0, 0, 0, 0);   // 9
  s21_decimal b = s21_dec_make(10, 0, 0, 1, 0);  // 1.0

  ck_assert_int_eq(s21_is_less(a, b), 0);
  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(eq_different_signs_non_zero) {
  s21_decimal a = s21_dec_make(1, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(1, 0, 0, 0, 1);

  ck_assert_int_eq(s21_is_equal(a, b), 0);
  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(less_sign_diff_negative_is_less) {
  s21_decimal neg = s21_dec_make(1, 0, 0, 0, 1);
  s21_decimal pos = s21_dec_make(1, 0, 0, 0, 0);

  ck_assert_int_eq(s21_is_less(neg, pos), 1);
  ck_assert_int_eq(s21_is_greater(neg, pos), 0);
}
END_TEST

START_TEST(greater_sign_diff_positive_is_greater) {
  s21_decimal neg = s21_dec_make(1, 0, 0, 0, 1);
  s21_decimal pos = s21_dec_make(1, 0, 0, 0, 0);

  ck_assert_int_eq(s21_is_greater(pos, neg), 1);
  ck_assert_int_eq(s21_is_less(pos, neg), 0);
}
END_TEST

START_TEST(less_same_sign_positive) {
  s21_decimal a = s21_dec_make(1, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(2, 0, 0, 0, 0);

  ck_assert_int_eq(s21_is_less(a, b), 1);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 0);
}
END_TEST

START_TEST(less_same_sign_negative) {
  s21_decimal a = s21_dec_make(2, 0, 0, 0, 1);  // -2
  s21_decimal b = s21_dec_make(1, 0, 0, 0, 1);  // -1

  ck_assert_int_eq(s21_is_less(a, b), 1);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(greater_same_sign_positive) {
  s21_decimal a = s21_dec_make(3, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(2, 0, 0, 0, 0);

  ck_assert_int_eq(s21_is_greater(a, b), 1);
  ck_assert_int_eq(s21_is_less(a, b), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 0);
}
END_TEST

START_TEST(le_and_ge_when_equal) {
  s21_decimal a = s21_dec_make(42, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(42, 0, 0, 0, 0);

  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
  ck_assert_int_eq(s21_is_less(a, b), 0);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(compare_high_mantissa) {
  s21_decimal max = s21_dec_make(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0, 0);
  s21_decimal one = s21_dec_make(1, 0, 0, 0, 0);

  ck_assert_int_eq(s21_is_greater(max, one), 1);
  ck_assert_int_eq(s21_is_less(one, max), 1);
}
END_TEST

Suite* s21_comparison_suite(void) {
  Suite* s = suite_create("comparison_operators");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, eq_zero_pos_vs_neg);
  tcase_add_test(tc, eq_scale_ignored_1_0_vs_1_00);
  tcase_add_test(tc, compare_same_value_different_scales_negative);
  tcase_add_test(tc, eq_different_signs_non_zero);
  tcase_add_test(tc, less_sign_diff_negative_is_less);
  tcase_add_test(tc, greater_sign_diff_positive_is_greater);
  tcase_add_test(tc, less_with_scale_alignment);
  tcase_add_test(tc, less_same_sign_positive);
  tcase_add_test(tc, less_same_sign_negative);
  tcase_add_test(tc, greater_same_sign_positive);
  tcase_add_test(tc, le_and_ge_when_equal);
  tcase_add_test(tc, compare_high_mantissa);

  suite_add_tcase(s, tc);
  return s;
}
