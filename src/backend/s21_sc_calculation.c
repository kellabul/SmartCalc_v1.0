#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../s21_smartcalc.h"

int thereIsXinInput(s_tokens *tokens);

int calculation(s_tokens *tokens, char *string) {
  token_stack stack = {};
  s_tokens final_result = {};
  if (thereIsXinInput(tokens)) {
    sprintf(string, "ENTER X VALUE");
  } else {
    stk_init(&stack);
    for (int i = 0; tokens[i].type != 0; i++) {
      if (tokens[i].type == S21_NUMBER || tokens[i].type == S21_VARIABLE) {
        stk_push(&stack, tokens[i]);
      } else if (operationRequaresOneNum(tokens[i].value)) {
        stk_push(&stack, calculateOneS21_NUMBER(&stack, tokens[i].value));
      } else {
        stk_push(&stack, calculateTwoS21_NUMBERs(&stack, tokens[i].value));
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
      sprintf(string, "%.7lf", final_result.value);
      deleteZeroesFromStringEnd(string);
      if (strlen(string) > 15)
        sprintf(string, "%.15e", final_result.value);
    }
  }
  return 0;
}

void deleteZeroesFromStringEnd(char *str) {
  for (size_t i = strlen(str) - 1; i != 0; i--) {
    if (str[i] == '.') str[i] = '\0';
    if (str[i] != '0') {
      str[i + 1] = '\0';
      break;
    }
  }
}

int thereIsXinInput(s_tokens *tokens) {
  int result = 0;
  for (int i = 0; tokens[i].type != 0; i++) {
    if (tokens[i].type == S21_VARIABLE) result = 1;
  }
  return result;
}

int operationRequaresOneNum(double value) {
  return (value == LN || value == LOG || value == COS || value == ACOS ||
          value == SIN || value == ASIN || value == TAN || value == ATAN ||
          value == SQRT);
}

s_tokens calculateTwoS21_NUMBERs(token_stack *stack, double operation) {
  s_tokens first = stk_pop(stack);
  s_tokens second = stk_pop(stack);
  s_tokens result = {.type = S21_NUMBER};

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

s_tokens calculateOneS21_NUMBER(token_stack *stack, double operation) {
  s_tokens result = {.type = S21_NUMBER};
  s_tokens NUMBER = stk_pop(stack);

  if (operation == LN)
    result.value = log(NUMBER.value);
  else if (operation == LOG)
    result.value = log10(NUMBER.value);
  else if (operation == COS)
    result.value = cos(NUMBER.value);
  else if (operation == SIN)
    result.value = sin(NUMBER.value);
  else if (operation == TAN)
    result.value = tan(NUMBER.value);
  else if (operation == ACOS)
    result.value = acos(NUMBER.value);
  else if (operation == ASIN)
    result.value = asin(NUMBER.value);
  else if (operation == ATAN)
    result.value = atan(NUMBER.value);
  else if (operation == SQRT)
    result.value = sqrt(NUMBER.value);
  return result;
}