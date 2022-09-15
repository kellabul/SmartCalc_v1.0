#include <math.h>
#include <stdio.h>
#include <string.h>

#include "s21_smartcalc.h"

// - Вход: общая сумма кредита, срок, процентная ставка, тип (аннуитетный,
// дифференцированный)
// - Выход: ежемесячный платеж, переплата по кредиту, общая выплата

double monthly_payment(double loan, double interestRate, int creditPeriod,
                      int type) {
  double payment = 0;
  double monthlyInterest = (interestRate / 100) / 12;
  //printf("mi %lf\n", monthlyInterest);
  if (type == ANNUITANTS) {
    payment =
        loan *
        (monthlyInterest +
         (monthlyInterest / ((pow((1 + monthlyInterest), creditPeriod) - 1))));
  } else if (type == DIFFERENTIATED) {
    //double repaidAmount = loan / creditPeriod;
    //double amountOfInterest = 
  } else {
    payment = INCORRECT_INPUT;
  }
  return payment;
}