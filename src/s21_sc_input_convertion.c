// change sin, cos, mod etc. to S, C, M ...
#include "s21_smartcalc.h"
#include <string.h>
#include <stdio.h>


/*
void input_conversion(char* input, s_tokens* ouput) {
  if (input_validation(input) != -1) {
    int i = 0, j = 0;
    if (input[0] == '-' || input[0] == '+') {
      ouput[j].type = 1;
      ouput[j++].value = getNumberFromString(input, &i);
    }
    for (; input[i]; i++, j++) {
      if(isNumber(&input[i])) {
        ouput[j].type = 1;
        ouput[j].value = getNumberFromString(input, &i);
      } else {
        ouput[j].type = 2;
      }
      int step = 0;
      if ((step = isLn(&input[i]))){
        i += step;
        ouput[j].value = LN;
      } else if ((step = isLog(&input[i]))) {
        i += step;
        ouput[j].value = LOG;
      } else if ((step = isCos(&input[i]))) {
        i += step;
        ouput[j].value = COS;
      } else if ((step = isSin(&input[i]))) {
        i += step;
        ouput[j].value = SIN;
      } else if ((step = isTan(&input[i]))) {
        i += step;
        ouput[j].value = TAN;
      } else if ((step = isSqrt(&input[i]))) {
        i += step;
        ouput[j].value = SQRT;
      } else if ((step = isAcos(&input[i]))) {
        i += step;
        ouput[j].value = ACOS;
      } else if ((step = isAsin(&input[i]))) {
        i += step;
        ouput[j].value = ASIN;
      } else if ((step = isAtan(&input[i]))) {
        i += step;
        ouput[j].value = ATAN;
      } else if ((step = isMod(&input[i]))) {
        i += step;
        ouput[j].value = MOD;
      } else {
        ouput[j].value = input[i];
      }
    }
    ouput[j].type = 0;
  }
}
*/

double getNumberFromString(char *string, int *i) {
  double result = 0;
  int step = 0;
  sscanf(string, "%lf", &result);
  while (isNumber(string[step]) || string[step] == '.') {
    step++;
  }
  *i += step;
  return result;
}
