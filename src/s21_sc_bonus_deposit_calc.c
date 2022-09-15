#include <math.h>

#include "s21_smartcalc.h"

/**
 *  - Вход: сумма вклада, срок размещения, процентная ставка, налоговая ставка,
 *  периодичность выплат, капитализация процентов, список пополнений, список
 *  частичных снятий
 *  - Выход: начисленные проценты, сумма налога, сумма на вкладе к концу срока
 **/

double ya_ne_znayu_poka(double deposit, int period, double interestRate,
                        double taxRate, int paymentFrequency,
                        int capitalization ) {
  double vozvrashaemoe = 0;
}