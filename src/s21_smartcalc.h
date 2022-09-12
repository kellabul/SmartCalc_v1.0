#ifndef SRC_S21_SMARTCALC_H_
#define SRC_S21_SMARTCALC_H_

#define MAX 256

typedef struct stack {
  char data[MAX];
  int top;
} stack;

typedef struct queue {
  double qu[MAX];
  int rear, front;
} queue;

typedef struct tokens {
  double value;
  char type;
} s_tokens;


enum funcs_and_mod {
  LN = 'l',
  LOG = 'L',
  COS = 'c',
  ACOS = 'C',
  SIN = 's',
  ASIN = 'S',
  TAN = 't',
  ATAN = 'T',
  SQRT = 'V',
  MOD = 'm'
};

// main functions
int input_validation(char *str);
void input_conversion(char* input, s_tokens* ouput);
void infix_to_postfix(char *infix, char *postfix);

// stack data structure
void stk_init(stack *s);
int stk_empty(stack *s);
int stk_full(stack *s);
int stk_push(stack *s, char value);
char stk_pop(stack *s);
char stk_top(stack *p);

// santa's little helpers
int isOperation(char element);
int isNumberOrX(char element);
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
int isWrongLastElement(char element);
int areWrongMiddleElements(char *str);
int isWrongMiddleElement(char *str);

//input conversion helpers
void checkUnarySign(char *input, s_tokens *output, int *i, int *j);
void convertUnarySign (char sign, s_tokens *token);
int getNumberFromString(char *string, double *value);
int convertFunction(char *str, double *value);

#endif  // SRC_S21_SMARTCALC_H_