#ifndef SRC_S21_SMARTCALC_H_
#define SRC_S21_SMARTCALC_H_

int input_validation(char *string);

#define MAX 250

typedef struct stack {
  int data[MAX];
  int top;
} stack;

// stack data structure
void init(stack *s);
int empty(stack *s);
int full(stack *s);
void push(stack *s, int x);
int pop(stack *s);
int top(stack *p);

// santa's little helpers
int isNumber(char element);
int isOperation(char element);

#endif  // SRC_S21_SMARTCALC_H_