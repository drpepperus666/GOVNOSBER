#include "test_common.h"

START_TEST(from_int_int_min) {
  s21_decimal d = {{0, 0, 0, 0}};
  int err = s21_from_int_to_decimal((-2147483647 - 1), &d);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(d, s21_dec_make(0x80000000u, 0, 0, 0, 1));
}
END_TEST

START_TEST(from_int_positive) {
  s21_decimal d = {{0, 0, 0, 0}};
  int err = s21_from_int_to_decimal(123, &d);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(d, s21_dec_make(123, 0, 0, 0, 0));
}
END_TEST

START_TEST(from_int_negative) {
  s21_decimal d = {{0, 0, 0, 0}};
  int err = s21_from_int_to_decimal(-456, &d);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(d, s21_dec_make(456, 0, 0, 0, 1));
}
END_TEST

START_TEST(from_int_zero) {
  s21_decimal d = {{1, 2, 3, 0x80000000u}};
  int err = s21_from_int_to_decimal(0, &d);
  ck_assert_int_eq(err, 0);
  ck_assert_decimal_eq(d, s21_dec_make(0, 0, 0, 0, 0));
}
END_TEST

START_TEST(from_int_null_dst) {
  int err = s21_from_int_to_decimal(7, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(from_int_sets_scale_zero) {
  s21_decimal d = {{0, 0, 0, (5u << 16)}};
  int err = s21_from_int_to_decimal(7, &d);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq((int)((d.bits[3] >> 16) & 0xFFu), 0);
}
END_TEST

Suite *s21_converters_suite(void) {
  Suite *s = suite_create("from_int_to_decimal");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, from_int_int_min);
  tcase_add_test(tc, from_int_positive);
  tcase_add_test(tc, from_int_negative);
  tcase_add_test(tc, from_int_zero);
  tcase_add_test(tc, from_int_null_dst);
  tcase_add_test(tc, from_int_sets_scale_zero);

  suite_add_tcase(s, tc);
  return s;
}
