#include <math.h>

#include "../s21_smartcalc.h"

// - Вход: общая сумма кредита, срок, процентная ставка, тип (аннуитетный,
// дифференцированный)
// - Выход: ежемесячный платеж, переплата по кредиту, общая выплата

long double total_payment(long double loan, long double interestRate, int creditPeriod,
                     int type, long double *firstPayment, long double *lastPayment) {
  long double totalPayment = 0;
  if (loan < 0.01 || interestRate < 0.01 || creditPeriod < 1 || !firstPayment ||
      !lastPayment) {
    totalPayment = S21_INCORRECT_INPUT;
  } else {
    long double payment = 0;
    long double monthlyInterest = (interestRate / 100) / 12;
    if (type == S21_ANNUITANTS) {
      payment = loan * (monthlyInterest +
                        (monthlyInterest /
                         ((pow((1 + monthlyInterest), creditPeriod) - 1))));
      *firstPayment = *lastPayment = payment;
      totalPayment = payment * creditPeriod;
    } else if (type == S21_DIFFERENTIATED) {
      long double mainPayment = loan / creditPeriod;
      long double alreadyRepaid = 0;
      long double restOfLoan = loan;
      for (int i = 0; i < creditPeriod; i++) {
        alreadyRepaid = mainPayment * i;
        restOfLoan = loan - alreadyRepaid;
        payment = mainPayment + restOfLoan * monthlyInterest;
        totalPayment += payment;
        if (i == 0) *firstPayment = payment;
        if (i == creditPeriod - 1) *lastPayment = payment;
      }
    } else {
      totalPayment = S21_INCORRECT_INPUT;
    }
  }
  return totalPayment;
}