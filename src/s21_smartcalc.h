#ifndef SRC_S21_SMARTCALC_H_
#define SRC_S21_SMARTCALC_H_

#define S21_MAX_INPUT 256
#define S21_MAX_TKN 2 * 256
#define S21_INCORRECT_INPUT -1
#define S21_TOO_MUCH_ARGS -2
#define S21_CORRECT_INPUT 0
#define S21_NUMBER -1
#define S21_VARIABLE -2
#define S21_ANNUITANTS 1
#define S21_DIFFERENTIATED 2

typedef struct {
  double value;
  int type;
} s_tokens;

typedef struct {
  s_tokens token[S21_MAX_TKN];
  int top;
} token_stack;

typedef struct {
  long double amount;
  int term;
  long double interest_rate;
  long double tax_rate;
  int periodicity;
  int capitalization;
  long double replenishment;
  int freq_of_replen;
  long double withdrawal;
  int freq_of_withd;
  // output
  long double accrued_interest;
  long double tax_amount;
  long double amount_by_end;
} s_deposit;

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
int input_conversion(char *input, s_tokens *output);
void infix_to_postfix(s_tokens *infix, s_tokens *postfix);
double calculation(char *input, double *x_value, char *string);

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
int isFunction(char *str);
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
void deleteZeroesFromStringEnd(char *str);
int isThereXinInput(s_tokens *tokens);
void replaceX(s_tokens *tokens, double x_value);

// another helpers
int isNotNumberInString(char *string);
void convertToFinancialOutputNumber(char *string, char *output);
void reverse_string(char *string);

// bonus 1 - credit calculator
long double total_payment(long double loan, long double interestRate, int term,
                          int type, long double *firstPayment,
                          long double *lastPayment);

// bonus 2
void deposit_calculation(s_deposit *deposit);
#endif  // SRC_S21_SMARTCALC_H_