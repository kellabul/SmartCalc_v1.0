#ifndef SRC_S21_SMARTCALC_H_
#define SRC_S21_SMARTCALC_H_

#define MAX 256

typedef struct stack {
  char data[MAX];
  int top;
} stack;

typedef struct queue {
  char qu[MAX];
  int rear, front;
} queue;

enum funcs_and_mod {
  LN = 100,
  LOG,
  COS,
  SIN,
  TAN,
  SQRT,
  ACOS,
  ASIN,
  ATAN,
  MOD
};

// main functions
int input_validation(char *string);

void input_processing(char *input, char *ouput);

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
int isMod(char *str);
int isWrongFirstElement(char *str);
int isWrongMiddleElement(char *str);
int isWrongLastElement(char element);

#endif  // SRC_S21_SMARTCALC_H_