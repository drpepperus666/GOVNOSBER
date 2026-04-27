#ifndef S21_INTERNAL_H
#define S21_INTERNAL_H

#include "s21_decimal.h"

void s21_znak_scale(int* znak_1, int* znak_2, int* scale_1, int* scale_2,
                    s21_decimal value_1, s21_decimal value_2);
void s21_mul_10(s21_decimal* min_scale, unsigned long long* overflow);
int s21_normalization(s21_decimal* min_scale, int* scale_1, int scale_2,
                      int znak);
void s21_minus(s21_decimal min_value, s21_decimal max_value, s21_decimal* res);
void s21_plus(unsigned long long* overflow, s21_decimal* res, s21_decimal val_1,
              s21_decimal val_2);
int s21_simplification(unsigned long long* overflow, int* scale_1,
                       s21_decimal* res, int znak);
void s21_multiplication(s21_decimal value_1, s21_decimal value_2,
                        s21_decimal* result, unsigned long long* overflow);
void s21_shift(s21_decimal* rem);
int s21_compare(s21_decimal remainder, s21_decimal value_2);
void s21_del_int(s21_decimal* remainder, s21_decimal value_1,
                 s21_decimal value_2, s21_decimal* result);
int s21_del_fraction(s21_decimal* remainder, s21_decimal value_2,
                     s21_decimal* result, int znak, int* scale);

#endif
