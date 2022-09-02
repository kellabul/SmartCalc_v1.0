#include "s21_smartcalc.h"

void init(stack *s) {  //
  s->top = -1;
}

int empty(stack *s) {  //
  return (s->top == -1 ? 1 : 0);
}

int full(stack *s) {  //
  return (s->top == (MAX - 1) ? 1 : 0);
}

void push(stack *s, int value) {
  s->top++;
  s->data[s->top] = value;
}

int pop(stack *s) {
  int result = s->data[s->top];
  s->top--;
  return result;
}

int top(stack *p) {  //
  return p->data[p->top];
}
