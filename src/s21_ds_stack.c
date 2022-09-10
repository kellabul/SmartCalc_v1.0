#include "s21_smartcalc.h"

void stk_init(stack *s) {  //
  s->top = 0;
}

int stk_empty(stack *s) {  //
  return (s->top == 0);
}

int stk_full(stack *s) {  //
  return (s->top == MAX - 1);
}

int stk_push(stack *s, char value) {
  char error = 0;
  if (!stk_full(s)) {
  s->data[s->top] = value;
  s->top++;
  } else {
    error = -1;
  }
  return error;
}

char stk_pop(stack *s) {
  char result = -1;
  if (!stk_empty(s)){
    s->top--;
    result = s->data[s->top];
  }
  return result;
}

char stk_top(stack *p) {  //
  return p->data[p->top];
}
