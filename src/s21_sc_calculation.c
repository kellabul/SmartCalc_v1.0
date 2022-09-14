#include <math.h>
#include <stdio.h>
#include <string.h>

#include "s21_smartcalc.h"

int calculation(s_tokens *tokens, char *string) {
  token_stack stack = {};
  s_tokens final_result = {};
  stk_init(&stack);
  for (int i = 0; tokens[i].type != 0; i++) {
    if (tokens[i].type == NUMBER || tokens[i].type == VARIABLE) {
      stk_push(&stack, tokens[i]);
    } else if (operationRequaresOneNum(tokens[i].value)) {
      stk_push(&stack, calculateOneNumber(&stack, tokens[i].value));
    } else {
      stk_push(&stack, calculateTwoNumbers(&stack, tokens[i].value));
    }
  }
  final_result = stk_pop(&stack);
  if (isinf(final_result.value)) {
    if (final_result.value < 0)
      sprintf(string, "- INFINITY");
    else
      sprintf(string, "INFINITY");
  } else if (isnan(final_result.value)) {
    sprintf(string, "NaN");
  } else {
    sprintf(string, "%lf", final_result.value);
  }
  deleteZeroesFromString(string);
  return 0;
}

void deleteZeroesFromString(char *str) {
  for (size_t i = strlen(str) - 1; i != 0; i--) {
    if (str[i] == '.') str[i] = '\0';
    if (str[i] != '0') {
      str[i + 1] = '\0';
      break;
    }
  }
}

int operationRequaresOneNum(double value) {
  return (value == LN || value == LOG || value == COS || value == ACOS ||
          value == SIN || value == ASIN || value == TAN || value == ATAN ||
          value == SQRT);
}

s_tokens calculateTwoNumbers(token_stack *stack, double operation) {
  s_tokens first = stk_pop(stack);
  s_tokens second = stk_pop(stack);
  s_tokens result = {.type = NUMBER};

  if (operation == '-')
    result.value = second.value - first.value;
  else if (operation == '+')
    result.value = second.value + first.value;
  else if (operation == '/')
    result.value = second.value / first.value;
  else if (operation == '*')
    result.value = second.value * first.value;
  else if (operation == '^')
    result.value = pow(second.value, first.value);
  else if (operation == MOD)
    result.value = calculateMod(second.value, first.value);
  return result;
}

double calculateMod(double left, double right) {
  double result;
  if ((left < 0 && right < 0) || (left > 0 && right > 0))
    result = fmod(left, right);
  else
    result = right + fmod(left, right);
  return result;
}

s_tokens calculateOneNumber(token_stack *stack, double operation) {
  s_tokens result = {.type = NUMBER};
  s_tokens number = stk_pop(stack);

  if (operation == LN)
    result.value = log(number.value);
  else if (operation == LOG)
    result.value = log10(number.value);
  else if (operation == COS)
    result.value = cos(number.value);
  else if (operation == SIN)
    result.value = sin(number.value);
  else if (operation == TAN)
    result.value = tan(number.value);
  else if (operation == ACOS)
    result.value = acos(number.value);
  else if (operation == ASIN)
    result.value = asin(number.value);
  else if (operation == ATAN)
    result.value = atan(number.value);
  else if (operation == SQRT)
    result.value = sqrt(number.value);
  return result;
}