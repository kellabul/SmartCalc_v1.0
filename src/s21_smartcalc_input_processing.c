// change sin, cos, mod etc. to S, C, M ...
#include "s21_smartcalc.h"

void input_processing(char* input, char* ouput) {
  for (int i = 0, j = 0; input[i]; i++, j++) {
    int step = 0;
    if ((step = isLn(&input[i]))) {
      i += step;
      ouput[j] = LN;
    } else if ((step = isLog(&input[i]))) {
      i += step;
      ouput[j] = LOG;
    } else if ((step = isCos(&input[i]))) {
      i += step;
      ouput[j] = COS;
    } else if ((step = isSin(&input[i]))) {
      i += step;
      ouput[j] = SIN;
    } else if ((step = isTan(&input[i]))) {
      i += step;
      ouput[j] = TAN;
    } else if ((step = isSqrt(&input[i]))) {
      i += step;
      ouput[j] = SQRT;
    } else if ((step = isAcos(&input[i]))) {
      i += step;
      ouput[j] = COS;
    } else if ((step = isAsin(&input[i]))) {
      i += step;
      ouput[j] = ASIN;
    } else if ((step = isAtan(&input[i]))) {
      i += step;
      ouput[j] = ATAN;
    } else if ((step = isMod(&input[i]))) {
      i += step;
      ouput[j] = MOD;
    } else {
      ouput[j] = input[i];
    }
  }
}
