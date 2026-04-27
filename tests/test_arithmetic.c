#include "test_common.h"

START_TEST(add_simple_positive) {
  s21_decimal a = s21_dec_make(2, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(3, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(5, 0, 0, 0, 0));
}
END_TEST

START_TEST(add_scale_normalization) {
  s21_decimal a = s21_dec_make(12, 0, 0, 1, 0);   // 1.2
  s21_decimal b = s21_dec_make(345, 0, 0, 2, 0);  // 3.45
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(465, 0, 0, 2, 0));  // 4.65
}
END_TEST

START_TEST(add_different_signs) {
  s21_decimal a = s21_dec_make(5, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(3, 0, 0, 0, 1);  // -3
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(2, 0, 0, 0, 0));
}
END_TEST

START_TEST(add_to_zero_clears_sign) {
  s21_decimal a = s21_dec_make(3, 0, 0, 0, 0);
  s21_decimal b = s21_dec_make(3, 0, 0, 0, 1);  // -3
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(add_pos_zero_plus_neg_zero_is_pos_zero) {
  s21_decimal pz = s21_dec_make(0, 0, 0, 0, 0);
  s21_decimal nz = s21_dec_make(0, 0, 0, 0, 1);
  s21_decimal res = {{1, 1, 1, 1}};

  int err = s21_add(pz, nz, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

Suite* s21_arithmetic_suite(void) {
  Suite* s = suite_create("add_sub_smoke");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, add_simple_positive);
  tcase_add_test(tc, add_scale_normalization);
  tcase_add_test(tc, add_different_signs);
  tcase_add_test(tc, add_to_zero_clears_sign);
  tcase_add_test(tc, add_pos_zero_plus_neg_zero_is_pos_zero);

  suite_add_tcase(s, tc);
  return s;
}
