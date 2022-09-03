#ifndef SRC_S21_SMARTCALC_H_
#define SRC_S21_SMARTCALC_H_

int input_validation(char *string);

#define MAX 256

typedef struct stack {
  char data[MAX];
  int top;
} stack;

typedef struct queue {
  char qu[MAX];
  int rear, front;
} queue;

// stack data structure
void stk_initialization(stack *s);
int stk_empty(stack *s);
int stk_full(stack *s);
int stk_push(stack *s, char value);
char stk_pop(stack *s);
char stk_top(stack *p);

// queue data structure


// santa's little helpers
int isNumber(char element);
int isOperation(char element);

#endif  // SRC_S21_SMARTCALC_H_