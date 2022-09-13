// change sin, cos, mod etc. to S, C, M ...
#include <stdio.h>
#include <string.h>

#include "s21_smartcalc.h"

/**
 *  token types:
 *  -2 == x (VARIABLE)
 *  -1 == numbers (NUMBER)
 *   1 == '+', '-' (min. priority)
 *   2 == '*', '\', '%'
 *   3 == '^'
 *   4 == trigonometric functions (max. pririty)
 *   5 == parentesis
 *   0 == end of expression
 **/

void input_conversion(char *input, s_tokens *output) {
  if (input_validation(input) != INCORRECT_INPUT) {
    int i = 0, j = 0;
    // check if first token is '+' or '-'
    checkUnarySign(&input[i], &output[j], &i, &j);
    // check if first token is 'x'
    if (input[0] == 'x') {
      setTokenType(&input[i], &output[j]);
      output[j++].value = input[i++];
    }
    for (; input[i]; i++, j++) {
      if (isNumber(input[i])) {
        setTokenType(&input[i], &output[j]);
        i += getNumberFromString(&input[i], &output[j].value);
      } else if (isTrigFunc(&input[i]) || isMod(&input[i])) {
        setTokenType(&input[i], &output[j]);
        i += convertFunction(&input[i], &output[j].value);
      } else if (input[i] == '(') {
        convertStringToTokens(&input[i], &output[j]);
        checkUnarySign(&input[i + 1], &output[j + 1], &i, &j);
      } else if (input[i] == 'x') {
        if (!isOperation(input[i - 1]) && input[i - 1] != '(' &&
            output[j - 1].value != MOD) {
          output[j].type = 2;
          output[j++].value = '*';
        }
        convertStringToTokens(&input[i], &output[j]);
      } else {
        convertStringToTokens(&input[i], &output[j]);
      }
    }
    // .type == 0 means the end of token line
    output[j].type = 0;
  }
}

void convertStringToTokens(char *string, s_tokens *token) {
  setTokenType(string, token);
  (*token).value = *string;
}

void checkUnarySign(char *input, s_tokens *output, int *i, int *j) {
  if (input[0] == '-' || input[0] == '+') {
    convertUnarySign(input[0], output);
    *i += 1;
    *j += 2;
  }
}

void convertUnarySign(char sign, s_tokens *token) {
  // converting -... to -1*...
  // if it passes validation, there are more than 2 tokens in input
  token[0].type = NUMBER;
  if (sign == '-') {
    token[0].value = -1;
  } else if (sign == '+') {
    token[0].value = 1;
  }
  token[1].type = 2;
  token[1].value = '*';
}

void setTokenType(char *string, s_tokens *token) {
  if (isNumber(*string))
    token->type = NUMBER;
  else if (*string == 'x')
    token->type = VARIABLE;
  else if (*string == '+' || *string == '-')
    token->type = 1;
  else if (*string == '*' || *string == '/' || isMod(string))
    token->type = 2;
  else if (*string == '^')
    token->type = 3;
  else if (isTrigFunc(string))
    token->type = 4;
  else if (*string == '(' || *string == ')')
    token->type = -5;
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
  return step - 1;
}
