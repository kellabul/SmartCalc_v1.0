#include <stdio.h>

#include "s21_smartcalc.h"
// printf("ili tuta string = %c \n", string[i]);

int input_validation(char *string) {
  int result = 0;
  int parenthesis = 0;
  for (int i = 0; string[i]; i++) {
    if (isOperation(string[i]) && !isNumber(string[i + 1])) {
      result = -1;
      break;
    } else if (string[i] == '(') {
      parenthesis++;
    } else if (string[i] == ')') {
      parenthesis--;
    } else if (!isNumber(string[i]) && !isOperation(string[i])) {
      result = -1;
      break;
    }
  }
  if (parenthesis) result = -1;
  return result;
}


int isOperation(char element) {
  return (element == '+' || element == '-' || element == '*' ||
          element == '/' || element == '^');
}

int isDot(char element) { return (element == '.'); }

int isNumber(char element) { return (element >= '0' && element <= '9'); }

int isLn(char *string) { return !strncmp(string, "ln", 2); }

int isLog(char *string) { return !strncmp(string, "log", 3); }

int isCos(char *string) { return !strncmp(string, "cos", 3); }

int isSin(char *string) { return !strncmp(string, "sin", 3); }

int isTan(char *string) { return !strncmp(string, "tan", 3); }

int isSqrt(char *string) { return !strncmp(string, "sqrt", 4); }

int isAcos(char *string) { return !strncmp(string, "acos", 4); }

int isAsin(char *string) { return !strncmp(string, "asin", 4); }

int isAtan(char *string) { return !strncmp(string, "atan", 4); }
