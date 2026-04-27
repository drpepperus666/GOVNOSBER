#include "test_common.h"

START_TEST(to_int_truncate_fraction_12_99_to_12) {
  s21_decimal v = s21_dec_make(1299, 0, 0, 2, 0);  // 12.99
  int out = 0;

  int err = s21_from_decimal_to_int(v, &out);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(out, 12);
}
END_TEST

START_TEST(to_int_negative_truncate_fraction) {
  s21_decimal v = s21_dec_make(199, 0, 0, 2, 1);  // -1.99
  int out = 0;

  int err = s21_from_decimal_to_int(v, &out);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(out, -1);
}
END_TEST

START_TEST(to_int_int_max_ok) {
  s21_decimal v = s21_dec_make(0x7FFFFFFFu, 0, 0, 0, 0);
  int out = 0;

  int err = s21_from_decimal_to_int(v, &out);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(out, 2147483647);
}
END_TEST

START_TEST(to_int_int_min_ok) {
  s21_decimal v = s21_dec_make(0x80000000u, 0, 0, 0, 1);
  int out = 0;

  int err = s21_from_decimal_to_int(v, &out);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(out, (-2147483647 - 1));
}
END_TEST

START_TEST(to_int_overflow_above_int_max_error) {
  s21_decimal v = s21_dec_make(0x80000000u, 0, 0, 0, 0);  // 2147483648
  int out = 123;

  int err = s21_from_decimal_to_int(v, &out);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(to_int_overflow_below_int_min_error) {
  s21_decimal v = s21_dec_make(0x80000001u, 0, 0, 0, 1);  // -(2147483649)
  int out = 123;

  int err = s21_from_decimal_to_int(v, &out);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(to_int_overflow_high_bits_error) {
  s21_decimal v = s21_dec_make(1, 1, 0, 0, 0);
  int out = 123;

  int err = s21_from_decimal_to_int(v, &out);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(to_int_null_dst_error) {
  s21_decimal v = s21_dec_make(1, 0, 0, 0, 0);
  int err = s21_from_decimal_to_int(v, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(to_int_scale_28_truncates_to_zero) {
  s21_decimal v = s21_dec_make(1, 0, 0, 28, 0);  // 1e-28
  int out = 123;
  int err = s21_from_decimal_to_int(v, &out);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(out, 0);
}
END_TEST

START_TEST(to_int_negative_scale_28_truncates_to_zero) {
  s21_decimal v = s21_dec_make(1, 0, 0, 28, 1);  // -1e-28
  int out = 123;
  int err = s21_from_decimal_to_int(v, &out);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(out, 0);
}
END_TEST

Suite *s21_converters_int_suite(void) {
  Suite *s = suite_create("from_decimal_to_int");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, to_int_truncate_fraction_12_99_to_12);
  tcase_add_test(tc, to_int_negative_truncate_fraction);
  tcase_add_test(tc, to_int_scale_28_truncates_to_zero);
  tcase_add_test(tc, to_int_negative_scale_28_truncates_to_zero);
  tcase_add_test(tc, to_int_int_max_ok);
  tcase_add_test(tc, to_int_int_min_ok);
  tcase_add_test(tc, to_int_overflow_above_int_max_error);
  tcase_add_test(tc, to_int_overflow_below_int_min_error);
  tcase_add_test(tc, to_int_overflow_high_bits_error);
  tcase_add_test(tc, to_int_null_dst_error);

  suite_add_tcase(s, tc);
  return s;
}
