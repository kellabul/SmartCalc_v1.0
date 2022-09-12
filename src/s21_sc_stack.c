#include "s21_smartcalc.h"

void stk_init(stack *stk) { //
  stk->top = 0;
}

int stk_empty(stack *stk) { //
  return (stk->top == 0);
}

int stk_full(stack *stk) { //
  return (stk->top == MAX_TKN - 1);
}

int stk_push(stack *stk, double value, int type) {
  char error = 0;
  if (!stk_full(stk) && stk) {
    stk->token[stk->top].value = value;
    stk->token[stk->top].type = type;
    stk->top++;
  } else {
    error = -1;
  }
  return error;
}

s_tokens stk_pop(stack *stk) {
  s_tokens result = {};
  if (!stk_empty(stk)) {
    stk->top--;
    result = stk->token[stk->top];
  }
  return result;
}

s_tokens stk_top(stack *stk) { //
  return stk->token[stk->top];
}
