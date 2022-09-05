#include <stdio.h>
#include <string.h>

int isNotElement(char *str);

#include "s21_smartcalc.h"
// printf("ili tuta str = %c \n", str[i]);

int input_validation(char *str) {
  int result = 0;
  int left_paren = 0;
  int right_paren = 0;
  if (isWrongFirstElement(str)) {
    result = -1;
    printf("isWrongFirstElement = %c \n", str[0]);
  } else {
    for (int i = 0; str[i]; i++) {
      int step = isTrigFunc(&str[i]);
      if (step) {
        i += step;
      } else if (str[i] == '(') {
        left_paren++;
      } else if (str[i] == ')') {
        if (right_paren + 1 > left_paren) {
          result = -1;
          printf("left par = %d right par = %d \n", left_paren, right_paren);
          break;
        } else {
          right_paren++;
        }
      } else if (isOperation(str[i]) && isNotElement(&str[i + 1])) {
        result = -1;
        printf("isWrongElement = %c \n", str[i]);
        break;
      } else if (!isNumber(str[i]) && !isOperation(str[i])) {
        printf("2 ili tuta str = %c \n", str[i]);
        result = -1;
        break;
      }
    }
  }
  if (left_paren != right_paren) result = -1;
  return result;
}

int isNotElement(char *str) {
  return str[0] != '(' && !isTrigFunc(&str[0]) && !isNumber(str[0]);
}

int isWrongFirstElement(char *str) {
  return str[0] != '-' && str[0] != '+' && str[0] != '(' &&
         !isTrigFunc(&str[0]) && !isNumber(str[0]);
}

int isOperation(char element) {
  return element == '+' || element == '-' || element == '*' || element == '/' ||
         element == '^';
}

int isTrigFunc(char *str) {
  int result = 0;
  if (isLn(str)) {
    result = 1;
  } else if (isLog(str) || isCos(str) || isSin(str) || isTan(str)) {
    result = 2;
  } else if (isSqrt(str) || isAcos(str) || isAsin(str) || isAtan(str)) {
    result = 3;
  }
  return result;
}

int isDot(char element) { return element == '.'; }

int isNumber(char element) {
  return (element >= '0' && element <= '9') || element == 'x';
}

int isLn(char *str) { return !strncmp(str, "ln", 2); }

int isLog(char *str) { return !strncmp(str, "log", 3); }

int isCos(char *str) { return !strncmp(str, "cos", 3); }

int isSin(char *str) { return !strncmp(str, "sin", 3); }

int isTan(char *str) { return !strncmp(str, "tan", 3); }

int isSqrt(char *str) { return !strncmp(str, "sqrt", 4); }

int isAcos(char *str) { return !strncmp(str, "acos", 4); }

int isAsin(char *str) { return !strncmp(str, "asin", 4); }

int isAtan(char *str) { return !strncmp(str, "atan", 4); }
