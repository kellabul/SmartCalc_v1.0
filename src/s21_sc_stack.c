#include "s21_smartcalc.h"

void stk_init(stack *stk) {  //
  stk->top = 0;
}

int stk_empty(stack *stk) {  //
  return (stk->top == 0);
}

int stk_full(stack *stk) {  //
  return (stk->top == MAX - 1);
}

int stk_push(stack *stk, double value, int type) {
  char error = 0;
  if (!stk_full(stk) && stk) {
  stk->token.value[stk->top] = value;
  stk->token.type[stk->top] = type;
  stk->top++;
  } else {
    error = -1;
  }
  return error;
}

char stk_pop(stack *stk) {
  char result = -1;
  if (!stk_empty(stk)){
    stk->top--;
    result = stk->data[stk->top];
  }
  return result;
}

char stk_top(stack *p) {  //
  return p->data[p->top];
}
