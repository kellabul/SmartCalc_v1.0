#include <math.h>

#include "../s21_smartcalc.h"

/**
 *
  - Input: total credit amount, term, interest rate, type (annuity,
differentiated)
  - Output: monthly payment, overpayment on credit, total payment
*
**/

long double total_payment(long double loan, long double interestRate, int term,
                          int type, long double *firstPayment,
                          long double *lastPayment) {
  long double totalPayment = 0;
  if (loan < 0.01 || interestRate < 0.01 || term < 1 || !firstPayment ||
      !lastPayment) {
    totalPayment = S21_INCORRECT_INPUT;
  } else {
    long double payment = 0;
    long double monthlyInterest = (interestRate / 100.00) / 12.00;
    if (type == S21_ANNUITANTS) {
      payment = loan *
                (monthlyInterest +
                 (monthlyInterest /
                  ((pow((1.00 + monthlyInterest), (long double)term) - 1.00))));

      /* formulae from wiki
            payment =
                loan * ((monthlyInterest * (pow((1.00 + monthlyInterest),
         term))) /
                        (((pow((1.00 + monthlyInterest), term) - 1.00))));
      */

      /* formulae from raiffeisen.ru
            payment =
                loan * (monthlyInterest /
                         (1 - (pow((1.00 + monthlyInterest), (long double)term *
         -1.00))));
      */

      *firstPayment = *lastPayment = payment;
      totalPayment = payment * term;
    } else if (type == S21_DIFFERENTIATED) {
      long double mainPayment = loan / term;
      long double alreadyRepaid = 0;
      long double restOfLoan = loan;
      for (int i = 0; i < term; i++) {
        alreadyRepaid = mainPayment * i;
        restOfLoan = loan - alreadyRepaid;
        payment = mainPayment + restOfLoan * monthlyInterest;
        totalPayment += payment;
        if (i == 0) *firstPayment = payment;
        if (i == term - 1) *lastPayment = payment;
      }
    } else {
      totalPayment = S21_INCORRECT_INPUT;
    }
  }
  return totalPayment;
}