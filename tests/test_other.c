#include "test_common.h"

START_TEST(truncate_scale_2_positive) {
  s21_decimal v = s21_dec_make(12345, 0, 0, 2, 0);  // 123.45
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_truncate(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(123, 0, 0, 0, 0));
}
END_TEST

START_TEST(truncate_scale_3_negative) {
  s21_decimal v = s21_dec_make(7001, 0, 0, 3, 1);  // -7.001
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_truncate(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(7, 0, 0, 0, 1));
}
END_TEST

START_TEST(truncate_zero_keeps_zero) {
  s21_decimal v = s21_dec_make(0, 0, 0, 10, 1);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_truncate(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(truncate_null_result) {
  s21_decimal v = s21_dec_make(123, 0, 0, 2, 0);
  int err = s21_truncate(v, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(negate_positive_number) {
  s21_decimal v = s21_dec_make(123, 0, 0, 0, 0);
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_negate(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(123, 0, 0, 0, 1));
}
END_TEST

START_TEST(negate_positive_zero_to_negative_zero) {
  s21_decimal z = s21_dec_make(0, 0, 0, 0, 0);
  s21_decimal res = {{1, 1, 1, 1}};

  int err = s21_negate(z, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_bits_eq(res, s21_dec_make(0, 0, 0, 0, 1));
}
END_TEST

START_TEST(negate_negative_zero_to_positive_zero) {
  s21_decimal z = s21_dec_make(0, 0, 0, 0, 1);
  s21_decimal res = {{1, 1, 1, 1}};

  int err = s21_negate(z, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_bits_eq(res, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(negate_null_result) {
  s21_decimal v = s21_dec_make(1, 0, 0, 0, 0);
  int err = s21_negate(v, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(floor_positive_truncates_fraction) {
  s21_decimal v = s21_dec_make(199, 0, 0, 2, 0);  // 1.99
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_floor(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(1, 0, 0, 0, 0));
}
END_TEST

START_TEST(floor_negative_fraction_goes_down) {
  s21_decimal v = s21_dec_make(11, 0, 0, 1, 1);  // -1.1
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_floor(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(2, 0, 0, 0, 1));  // -2
}
END_TEST

START_TEST(floor_negative_integer_keeps_value) {
  s21_decimal v = s21_dec_make(7, 0, 0, 0, 1);  // -7
  s21_decimal res = {{0, 0, 0, 0}};

  int err = s21_floor(v, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(7, 0, 0, 0, 1));
}
END_TEST

START_TEST(floor_null_result) {
  s21_decimal v = s21_dec_make(1, 0, 0, 1, 0);
  int err = s21_floor(v, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(floor_negative_zero_is_zero) {
  s21_decimal nz = s21_dec_make(0, 0, 0, 0, 1);
  s21_decimal res = {{1, 1, 1, 1}};

  int err = s21_floor(nz, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(truncate_negative_zero_is_zero) {
  s21_decimal nz = s21_dec_make(0, 0, 0, 10, 1);
  s21_decimal res = {{1, 1, 1, 1}};

  int err = s21_truncate(nz, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(res, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

Suite *s21_other_suite(void) {
  Suite *s = suite_create("other_floor_truncate_negate");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, truncate_scale_2_positive);
  tcase_add_test(tc, truncate_scale_3_negative);
  tcase_add_test(tc, truncate_zero_keeps_zero);
  tcase_add_test(tc, truncate_null_result);
  tcase_add_test(tc, negate_positive_number);
  tcase_add_test(tc, negate_positive_zero_to_negative_zero);
  tcase_add_test(tc, negate_negative_zero_to_positive_zero);
  tcase_add_test(tc, negate_null_result);
  tcase_add_test(tc, floor_positive_truncates_fraction);
  tcase_add_test(tc, floor_negative_fraction_goes_down);
  tcase_add_test(tc, floor_negative_integer_keeps_value);
  tcase_add_test(tc, floor_null_result);
  tcase_add_test(tc, floor_negative_zero_is_zero);
  tcase_add_test(tc, truncate_negative_zero_is_zero);

  suite_add_tcase(s, tc);
  return s;
}
