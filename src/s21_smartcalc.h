#ifndef SRC_S21_SMARTCALC_H_
#define SRC_S21_SMARTCALC_H_

#define MAX_INPUT 256
#define MAX_TKN 2 * 256
#define INCORRECT_INPUT -1
#define CORRECT_INPUT 0
#define NUMBER -1
#define VARIABLE -2

typedef struct tokens {
  double value;
  int type;
} s_tokens;

typedef struct s_stack {
  s_tokens token[MAX_TKN];
  int top;
} token_stack;

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
void input_conversion(char *input, s_tokens *ouput);
void infix_to_postfix(s_tokens *infix, s_tokens *postfix);
int calculation(s_tokens *tokens, char *string);

// stack data structure
void stk_init(token_stack *s);
int stk_empty(token_stack *s);
int stk_full(token_stack *s);
void stk_push(token_stack *stk, s_tokens element);
s_tokens stk_pop(token_stack *stk);
int stk_top_type(token_stack *stk);

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

// input conversion helpers
void checkUnarySign(char *input, s_tokens *output, int *i, int *j);
void convertUnarySign(char sign, s_tokens *token);
int getNumberFromString(char *string, double *value);
int convertFunction(char *str, double *value);

// helpers to work with s_tokens
void setTokenType(char *string, s_tokens *token);
void convertTokensToString(s_tokens *tokens, char *string);
void convertStringToTokens(char *string, s_tokens *token);

// resverse notation helpers
int pullTokensWithHigherPriority(int priority, token_stack *stack,
                                 s_tokens *postfix);
int pullContentOfParents(token_stack *stack, s_tokens *postfix);
void addTokensFromStackToPostfixLine(token_stack *stack, s_tokens *postfix);

// calculation helpers

int operationRequaresOneNum(double value);
s_tokens calculateOneNumber(token_stack *stack, double operation);
s_tokens calculateTwoNumbers(token_stack *stack, double operation);
double calculateMod(double second, double first);

#endif  // SRC_S21_SMARTCALC_H_