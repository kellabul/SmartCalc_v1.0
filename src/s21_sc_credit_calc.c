#include <math.h>
#include <stdio.h>
#include <string.h>

#include "s21_smartcalc.h"

// - Вход: общая сумма кредита, срок, процентная ставка, тип (аннуитетный,
// дифференцированный)
// - Выход: ежемесячный платеж, переплата по кредиту, общая выплата

double total_payment(double loan, double interestRate, int creditPeriod,
                       int type, double *firstPayment, double *lastPayment) {
  double totalPayment = 0;
  if (loan < 0.01 || interestRate < 0.01 || creditPeriod < 1 || !firstPayment || !lastPayment) {
    totalPayment = INCORRECT_INPUT;
  } else {
    double payment = 0;
    double monthlyInterest = (interestRate / 100) / 12;
    // printf("mi %lf\n", monthlyInterest);
    if (type == ANNUITANTS) {
        payment =
            loan *
            (monthlyInterest +
            (monthlyInterest / ((pow((1 + monthlyInterest), creditPeriod) - 1))));
        *firstPayment = *lastPayment = payment;
        totalPayment = payment * creditPeriod;
    } else if (type == DIFFERENTIATED) {
        double mainPayment = loan / creditPeriod;
        double alreadyRepaid = 0;
        double restOfLoan = loan;
        for (int i = 0; i < creditPeriod; i++) {
            alreadyRepaid = mainPayment * i;
            restOfLoan = loan - alreadyRepaid;
            payment = mainPayment + restOfLoan * monthlyInterest;
            totalPayment += payment;
            if (i == 0) *firstPayment = payment;
            if (i == creditPeriod - 1) *lastPayment = payment;
            //printf("%lf ostatok %lf\n", payment, restOfLoan);
        }
    } else {
        totalPayment = INCORRECT_INPUT;
    }
  }
  return totalPayment;
}