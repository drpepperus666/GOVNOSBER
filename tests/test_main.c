#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

Suite *s21_arithmetic_suite(void);
Suite *s21_arithmetic_complex_suite(void);
Suite *s21_arithmetic_basic_suite(void);
Suite *s21_comparison_suite(void);
Suite *s21_converters_suite(void);
Suite *s21_converters_int_suite(void);
Suite *s21_float_converters_suite(void);
Suite *s21_other_suite(void);
Suite *s21_round_suite(void);

static int run_module(Suite *suite, const char *function_name) {
  SRunner *sr = srunner_create(suite);
  srunner_set_fork_status(sr, CK_NOFORK);

  int null_fd = open("/dev/null", O_WRONLY);
  int saved_stdout = dup(STDOUT_FILENO);

  if (null_fd >= 0) {
    dup2(null_fd, STDOUT_FILENO);
    close(null_fd);
  }

  srunner_run_all(sr, CK_NORMAL);

  if (saved_stdout >= 0) {
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
  }

  int run = srunner_ntests_run(sr);
  int failed = srunner_ntests_failed(sr);

  int passed = run - failed;
  int percent = (run > 0) ? (passed * 100) / run : 100;

  printf("\n════════════════════════════════════════════════════════════\n");
  printf("[ %-40s ] %3d%%\n", function_name, percent);
  printf("  Checks: %d | Failures: %d | Passed: %d\n", run, failed, passed);
  printf("════════════════════════════════════════════════════════════\n");

  srunner_free(sr);
  return failed;
}

int main(void) {
  int failed_total = 0;

  printf("\n");
  printf("╔════════════════════════════════════════════════════════════╗\n");
  printf("║           S21 DECIMAL LIBRARY TEST RESULTS                ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n");

  failed_total += run_module(s21_arithmetic_suite(), "s21_add");
  failed_total += run_module(s21_arithmetic_complex_suite(), "s21_mul");
  failed_total += run_module(s21_arithmetic_basic_suite(), "s21_sub");
  failed_total += run_module(s21_comparison_suite(),
                             "s21_is_less/greater/equal");
  failed_total += run_module(s21_converters_suite(), "s21_from_int_to_decimal");
  failed_total += run_module(s21_converters_int_suite(),
                             "s21_from_decimal_to_int");
  failed_total += run_module(s21_float_converters_suite(),
                             "s21_from_float_to_decimal");
  failed_total += run_module(s21_other_suite(),
                             "s21_truncate/negate/floor");
  failed_total += run_module(s21_round_suite(), "s21_round");

  printf("\n");
  printf("╔════════════════════════════════════════════════════════════╗\n");
  if (failed_total == 0) {
    printf("║              ✓ ALL TESTS PASSED SUCCESSFULLY ✓             ║\n");
  } else {
    printf("║           ✗ %d TEST(S) FAILED - CHECK ABOVE ✗           ║\n",
           failed_total);
  }
  printf("╚════════════════════════════════════════════════════════════╝\n\n");

  return (failed_total == 0) ? 0 : 1;
}
