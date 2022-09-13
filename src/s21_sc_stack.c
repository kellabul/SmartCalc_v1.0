#include "s21_smartcalc.h"

void stk_init(token_stack *stk) { //
  stk->top = 0;
}

int stk_empty(token_stack *stk) { //
  return (stk->top == 0);
}

int stk_full(token_stack *stk) { //
  return (stk->top == MAX_TKN - 1);
}

int stk_push(token_stack *stk, s_tokens element) {
  char error = 0;
  if (!stk_full(stk) && stk) {
    stk->token[stk->top] = element;
    stk->top++;
  } else {
    error = -1;
  }
  return error;
}

s_tokens stk_pop(token_stack *stk) {
  s_tokens result = {};
  if (!stk_empty(stk)) {
    stk->top--;
    result = stk->token[stk->top];
  }
  return result;
}

int stk_top_type(token_stack *stk) { //
  int index_of_top_token = stk->top-1;
  return stk->token[index_of_top_token].type;
}