#include "test_common.h"

START_TEST(round_bankers_0_5_to_0) {
  s21_decimal v = s21_dec_make(5, 0, 0, 1, 0);  // 0.5
  s21_decimal res = {{0, 0, 0, 0}};
  int err = s21_round(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(round_bankers_1_5_to_2) {
  s21_decimal v = s21_dec_make(15, 0, 0, 1, 0);  // 1.5
  s21_decimal res = {{0, 0, 0, 0}};
  int err = s21_round(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(2, 0, 0, 0, 0));
}
END_TEST

START_TEST(round_bankers_2_5_to_2) {
  s21_decimal v = s21_dec_make(25, 0, 0, 1, 0);  // 2.5
  s21_decimal res = {{0, 0, 0, 0}};
  int err = s21_round(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(2, 0, 0, 0, 0));
}
END_TEST

START_TEST(round_bankers_3_5_to_4) {
  s21_decimal v = s21_dec_make(35, 0, 0, 1, 0);  // 3.5
  s21_decimal res = {{0, 0, 0, 0}};
  int err = s21_round(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(4, 0, 0, 0, 0));
}
END_TEST

START_TEST(round_bankers_neg_0_5_to_0) {
  s21_decimal v = s21_dec_make(5, 0, 0, 1, 1);  // -0.5
  s21_decimal res = {{0, 0, 0, 0}};
  int err = s21_round(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(round_bankers_neg_1_5_to_neg_2) {
  s21_decimal v = s21_dec_make(15, 0, 0, 1, 1);  // -1.5
  s21_decimal res = {{0, 0, 0, 0}};
  int err = s21_round(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(2, 0, 0, 0, 1));
}
END_TEST

START_TEST(round_bankers_neg_2_5_to_neg_2) {
  s21_decimal v = s21_dec_make(25, 0, 0, 1, 1);  // -2.5
  s21_decimal res = {{0, 0, 0, 0}};
  int err = s21_round(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(2, 0, 0, 0, 1));
}
END_TEST

START_TEST(round_integer_keeps_scale_zero) {
  s21_decimal v = s21_dec_make(123, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};
  int err = s21_round(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(123, 0, 0, 0, 0));
}
END_TEST

START_TEST(round_large_scale_positive) {
  s21_decimal v = s21_dec_make(12345678u, 0, 0, 7, 0);  // 1.2345678
  s21_decimal res = {{0, 0, 0, 0}};
  int err = s21_round(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(1, 0, 0, 0, 0));
}
END_TEST

START_TEST(round_large_scale_negative) {
  s21_decimal v = s21_dec_make(12345678u, 0, 0, 7, 1);  // -1.2345678
  s21_decimal res = {{0, 0, 0, 0}};
  int err = s21_round(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(1, 0, 0, 0, 1));
}
END_TEST

START_TEST(round_null_result) {
  s21_decimal v = s21_dec_make(15, 0, 0, 1, 0);
  int err = s21_round(v, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST

Suite *s21_round_suite(void) {
  Suite *s = suite_create("round_bankers");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, round_bankers_0_5_to_0);
  tcase_add_test(tc, round_bankers_1_5_to_2);
  tcase_add_test(tc, round_bankers_2_5_to_2);
  tcase_add_test(tc, round_bankers_3_5_to_4);
  tcase_add_test(tc, round_bankers_neg_0_5_to_0);
  tcase_add_test(tc, round_bankers_neg_1_5_to_neg_2);
  tcase_add_test(tc, round_bankers_neg_2_5_to_neg_2);
  tcase_add_test(tc, round_integer_keeps_scale_zero);
  tcase_add_test(tc, round_large_scale_positive);
  tcase_add_test(tc, round_large_scale_negative);
  tcase_add_test(tc, round_null_result);

  suite_add_tcase(s, tc);
  return s;
}
