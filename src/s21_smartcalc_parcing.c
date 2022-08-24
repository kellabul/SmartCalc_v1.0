#include "s21_smartcalc.h"

#include <stdio.h>
      //printf("ili tuta string = %c \n", string[i]);

int input_validation(char *string) {
  int result = 0;
  int braces = 0;
  for (int i = 0; string[i]; i++) {
    if (isOperation(string[i]) && !isNumber(string[i + 1])) {
      result = -1;
      break;
    } else if (string[i] == '(') {
      braces++;
    } else if (string[i] == ')') {
      braces--;
    } else if (!isNumber(string[i]) && !isOperation(string[i])) {
      result = -1;
      break;
    }
  }
  if (braces) result = -1;
  return result;
}

int isOperation(char element) {
  int result = 0;
  if (element == '+' || element == '-' || element == '*' || element == '/') result = 1;
  return result;
}

int isNumber(char element) {
  int result = 0;
  if (element >= '0' && element <= '9') result = 1;
  return result;
}