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
int isOperation(char element);
int isNumber(char element);
int isTrigFunc(char *str);
int isLn(char *str);
int isLog(char *str);
int isCos(char *str);
int isSin(char *str);
int isTan(char *str);
int isSqrt(char *str);
int isAcos(char *str);
int isAsin(char *str);
int isAtan(char *str);
int isWrongFirstElement(char *str);
int isWrongMiddleElement(char *str);
int isWrongLastElement(char element);

#endif  // SRC_S21_SMARTCALC_H_