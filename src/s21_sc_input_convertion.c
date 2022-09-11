// change sin, cos, mod etc. to S, C, M ...
#include <stdio.h>
#include <string.h>

#include "s21_smartcalc.h"

void input_conversion(char *input, s_tokens *ouput) {
  if (input_validation(input) != -1) {
    int i = 0, j = 0;
    // check if first token is '+' or '-'
    if (input[0] == '-' || input[0] == '+') {
      ouput[j].type = 1;
      i += getNumberFromString(input, &ouput[j++].value);
    }
    for (; input[i]; i++, j++) {
      if (isNumber(&input[i])) {
        ouput[j].type = 1;
        i += getNumberFromString(input, &ouput[j].value);
      } else if (isTrigFunc(&input[i])) {
        ouput[j].type = 2;
        i += convertFunction(&input[i], &ouput[j].value);
      } else if (input[i] == '(') {
        ouput[j].type = 2;
        ouput[j].value = '(';
        if (input[i+1] == '-' || input[i+1] == '+') {
          ouput[++j].type = 1;
          i += getNumberFromString(input, &ouput[j].value);
        }
      } else if (input[i] == ')') {
        ouput[j].type = 2;
        ouput[j].value = ')';
      } else if (input[i] == 'x') {
        if (i > 0) {
          if (input[i - 1] == '-') 
        }
      }

    }
    ouput[j].type = 0;
  }
}

int convertFunction(char *str, double *value) {
  int step = 0;
  if ((step = isLn(str))) {
    *value = LN;
  } else if ((step = isLog(str))) {
    *value = LOG;
  } else if ((step = isCos(str))) {
    *value = COS;
  } else if ((step = isSin(str))) {
    *value = SIN;
  } else if ((step = isTan(str))) {
    *value = TAN;
  } else if ((step = isSqrt(str))) {
    *value = SQRT;
  } else if ((step = isAcos(str))) {
    *value = ACOS;
  } else if ((step = isAsin(str))) {
    *value = ASIN;
  } else if ((step = isAtan(str))) {
    *value = ATAN;
  } else if ((step = isMod(str))) {
    *value = MOD;
  }
  return step;
}

int getNumberFromString(char *string, double *value) {
  int step = 0;
  sscanf(string, "%lf", value);
  while (isNumber(string[step]) || string[step] == '.') {
    step++;
  }
  return step;
}
