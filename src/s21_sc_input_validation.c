#include <stdio.h>
#include <string.h>

#include "s21_smartcalc.h"
// printf("ili tuta str = %c \n", str[i]);

int input_validation(char *str) {
  int result = CORRECT_INPUT;
  if (isWrongFirstElement(str)) {
    result = INCORRECT_INPUT;
  } else if (isWrongLastElement(str[strlen(str) - 1])) {
    result = INCORRECT_INPUT;
  } else if (areWrongMiddleElements(str)) {
    result = INCORRECT_INPUT;
  }
  return result;
}

int isWrongFirstElement(char *str) {
  return str[0] != '-' && str[0] != '+' && isWrongMiddleElement(str);
}

int isWrongMiddleElement(char *str) {
  return str[0] != '(' && !isTrigFunc(&str[0]) && !isNumberOrX(str[0]);
}

int isWrongLastElement(char element) {
  return element != ')' && !isNumberOrX(element);
}

int areWrongMiddleElements(char *str) {
  int result = 0, left_paren = 0, right_paren = 0;

  for (int i = 0; str[i] && result == 0; i++) {
    int trigFuncLength = isTrigFunc(&str[i]);
    int modLength = isMod(&str[i]);
    if (trigFuncLength) {
      i += trigFuncLength;
      if (str[i + 1] != '(') {
        result = -1;
      }
    } else if (modLength) {
      i += modLength;
      if (isWrongMiddleElement(&str[i + 1])) {
        result = -1;
      }
    } else if (str[i] == '(') {
      if (isWrongFirstElement(&str[i + 1])) {
        result = -1;
      } else {
        left_paren++;
      }
    } else if (str[i] == ')') {
      if (right_paren + 1 > left_paren) {
        result = -1;
      } else {
        right_paren++;
      }
    } else if (isOperation(str[i]) && isWrongMiddleElement(&str[i + 1])) {
      result = -1;
    } else if (str[i] == '.' && !isNumberOrX(str[i + 1]) &&
               !isOperation(str[i - 1])) {
      result = -1;
    } else if (str[i] == 'x' && !isOperation(str[i + 1]) && str[i + 1] != ')') {
      result = -1;
    } else if (!isNumberOrX(str[i]) && !isOperation(str[i]) && str[i] != '.') {
      result = -1;
    }
  }
  if (left_paren != right_paren)
    result = -1;
  return result;
}

int isOperation(char element) {
  return element == '+' || element == '-' || element == '*' || element == '/' ||
         element == '^';
}

int isTrigFunc(char *str) {
  // result depends on the function name length
  int result = 0;
  if ((result = isLn(str))) {
  } else if ((result = isLog(str))) {
  } else if ((result = isCos(str))) {
  } else if ((result = isSin(str))) {
  } else if ((result = isTan(str))) {
  } else if ((result = isSqrt(str))) {
  } else if ((result = isAcos(str))) {
  } else if ((result = isAsin(str))) {
  } else if ((result = isAtan(str))) {
  }
  return result;
}

int isNumberOrX(char element) { return isNumber(element) || element == 'x'; }

int isNumber(char element) { return element >= '0' && element <= '9'; }

int isLn(char *str) { return !strncmp(str, "ln", 2); }

int isMod(char *str) { return 2 * !strncmp(str, "mod", 3); }

int isLog(char *str) { return 2 * !strncmp(str, "log", 3); }

int isCos(char *str) { return 2 * !strncmp(str, "cos", 3); }

int isSin(char *str) { return 2 * !strncmp(str, "sin", 3); }

int isTan(char *str) { return 2 * !strncmp(str, "tan", 3); }

int isSqrt(char *str) { return 3 * !strncmp(str, "sqrt", 4); }

int isAcos(char *str) { return 3 * !strncmp(str, "acos", 4); }

int isAsin(char *str) { return 3 * !strncmp(str, "asin", 4); }

int isAtan(char *str) { return 3 * !strncmp(str, "atan", 4); }
