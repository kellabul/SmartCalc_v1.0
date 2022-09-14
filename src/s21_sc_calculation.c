#include <math.h>
#include <stdio.h>
#include "s21_smartcalc.h"

int calculation(s_tokens *tokens, char *string) {
  token_stack stack = {};
  stk_init(&stack);
  for (int i = 0; tokens[i].type != 0; i++) {
    if (tokens[i].type == NUMBER || tokens[i].type == VARIABLE)
      stk_push(&stack, tokens[i]);
    else if (tokens[i].value == '-')
      stk_push(&stack, token_substraction(&stack));
    else if (tokens[i].value == '+')
      stk_push(&stack, token_addition(&stack));
    else if (tokens[i].value == '/')
      stk_push(&stack, token_division(&stack));
    else if (tokens[i].value == '*')
      stk_push(&stack, token_multiplication(&stack));
  }
  s_tokens final = stk_pop(&stack);
  double result = final.value;
  sprintf(string, "%g", result);
  return 0;
}

s_tokens token_substraction(token_stack *stack) {
  s_tokens number_1 = stk_pop(stack);
  s_tokens number_2 = stk_pop(stack);
  s_tokens result;
  result.value = number_2.value - number_1.value;
  return result;
}

s_tokens token_addition(token_stack *stack) {
  s_tokens number_1 = stk_pop(stack);
  s_tokens number_2 = stk_pop(stack);
  s_tokens result;
  result.value = number_2.value + number_1.value;
  return result;
}

s_tokens token_division(token_stack *stack) {
  s_tokens number_1 = stk_pop(stack);
  s_tokens number_2 = stk_pop(stack);
  s_tokens result;
  result.value = number_2.value / number_1.value;
  return result;
}

s_tokens token_multiplication(token_stack *stack) {
  s_tokens number_1 = stk_pop(stack);
  s_tokens number_2 = stk_pop(stack);
  s_tokens result;
  result.value = number_2.value * number_1.value;
  return result;
}
