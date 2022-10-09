#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../s21_smartcalc.h"

double calculation(char *input, double *x_value, char *string) {
  token_stack stack = {};
  s_tokens final_result = {};
  s_tokens infix[S21_MAX_TKN] = {};
  s_tokens postfix[S21_MAX_TKN] = {};
  if (input_conversion(input, infix) == S21_INCORRECT_INPUT) {
    sprintf(string, "INCORRECT INPUT");
  } else if (isThereXinInput(infix) && x_value == NULL) {
    sprintf(string, "ENTER X VALUE");
    // if x_value doesn't pass valifation in gtk input, it equals to infinity
  } else if (isThereXinInput(infix) && isinf(*x_value)) {
    sprintf(string, "WRONG X VALUE");
  } else {
    if (x_value != NULL) replaceX(infix, *x_value);
    infix_to_postfix(infix, postfix);
    stk_init(&stack);
    for (int i = 0; postfix[i].type != 0; i++) {
      if (postfix[i].type == S21_NUMBER || postfix[i].type == S21_VARIABLE) {
        stk_push(&stack, postfix[i]);
      } else if (operationRequaresOneNum(postfix[i].value)) {
        stk_push(&stack, calculateOneNumber(&stack, postfix[i].value));
      } else {
        stk_push(&stack, calculateTwoNumbers(&stack, postfix[i].value));
      }
    }
    final_result = stk_pop(&stack);
    if (string != NULL) {
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
        if (strlen(string) > 15) sprintf(string, "%.9e", final_result.value);
      }
    }
  }
  return final_result.value;
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

void replaceX(s_tokens *tokens, double x_value) {
  for (int i = 0; tokens[i].type != 0; i++) {
    if (tokens[i].type == S21_VARIABLE) {
      tokens[i].type = S21_NUMBER;
      tokens[i].value = x_value;
    };
  }
}

int isThereXinInput(s_tokens *tokens) {
  int result = 0;
  for (int i = 0; tokens[i].type != 0; i++) {
    if (tokens[i].type == S21_VARIABLE) {
      result = 1;
      break;
    }
  }
  return result;
}

int operationRequaresOneNum(double value) {
  return (value == LN || value == LOG || value == COS || value == ACOS ||
          value == SIN || value == ASIN || value == TAN || value == ATAN ||
          value == SQRT);
}

s_tokens calculateTwoNumbers(token_stack *stack, double operation) {
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

s_tokens calculateOneNumber(token_stack *stack, double operation) {
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