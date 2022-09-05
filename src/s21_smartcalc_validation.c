#include <string.h>

#include "s21_smartcalc.h"
// printf("ili tuta str = %c \n", str[i]);

int input_validation(char *str) {
  int result = 0;
  int parenthesis = 0;
  for (int i = 0; str[i]; i++) {
    int step = isTrigFunc(&str[i]);
    if (step) {
      i += step;
    } else if (str[i] == '(') {
      parenthesis++;
    } else if (str[i] == ')') {
      parenthesis--;
    } else if (isOperation(str[i]) && !isNumber(str[i + 1])) {
      result = -1;
      break;
    } else if (!isNumber(str[i]) && !isOperation(str[i])) {
      result = -1;
      break;
    }
  }
  if (parenthesis) result = -1;
  if (str[0] != '-' || str[0] != '+' || !isTrigFunc(&str[0]) || isNumber(str[0])) result = -1;
  return result;
}

int isOperation(char element) {
  return (element == '+' || element == '-' || element == '*' || element == '/' || element == '^');
}

int isTrigFunc(char *str) {
  int result = 0;
  if (isLn(str)) {
    result = 2;
  } else if (isLog(str) || isCos(str) || isSin(str) || isTan(str)) {
    result = 3;
  } else if (isSqrt(str) || isAcos(str) || isAsin(str) || isAtan(str)) {
    result = 4;
  }
  return result;
}

int isDot(char element) { return (element == '.'); }

int isNumber(char element) { return (element >= '0' && element <= '9'); }

int isLn(char *str) { return !strncmp(str, "ln", 2); }

int isLog(char *str) { return !strncmp(str, "log", 3); }

int isCos(char *str) { return !strncmp(str, "cos", 3); }

int isSin(char *str) { return !strncmp(str, "sin", 3); }

int isTan(char *str) { return !strncmp(str, "tan", 3); }

int isSqrt(char *str) { return !strncmp(str, "sqrt", 4); }

int isAcos(char *str) { return !strncmp(str, "acos", 4); }

int isAsin(char *str) { return !strncmp(str, "asin", 4); }

int isAtan(char *str) { return !strncmp(str, "atan", 4); }
