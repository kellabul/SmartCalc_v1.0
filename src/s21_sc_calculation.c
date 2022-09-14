#include <math.h>
#include <stdio.h>

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
    if (final_result.value < 0) sprintf(string, "-infinity");
    else sprintf(string, "infinity");
  } else if (isnan(final_result.value)) {
    sprintf(string, "Nan");
  } else {
    sprintf(string, "%g", final_result.value);
  }
  return 0;
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
  else if (operation == '^')
    result.value = pow(second.value, first.value);
  else if (operation == MOD)
    result.value = fmod(second.value, first.value);
  return result;
}

s_tokens calculateOneNumber(token_stack *stack, double operation) {
  s_tokens result = {.type = NUMBER};
  s_tokens first = stk_pop(stack);

  if (operation == LN)
    result.value = log(first.value);
  else if (operation == LOG)
    result.value = log10(first.value);
  else if (operation == COS)
    result.value = cos(first.value);
  else if (operation == SIN)
    result.value = sin(first.value);
  else if (operation == TAN)
    result.value = tan(first.value);
  else if (operation == ACOS)
    result.value = acos(first.value);
  else if (operation == ASIN)
    result.value = asin(first.value);
  else if (operation == ATAN)
    result.value = atan(first.value);
  else if (operation == SQRT)
    result.value = sqrt(first.value);
  return result;
}