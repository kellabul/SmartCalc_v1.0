#include "../s21_smartcalc.h"

void infix_to_postfix(s_tokens *infix, s_tokens *postfix) {
  token_stack stack = {};
  stk_init(&stack);
  int j = 0;
  for (int i = 0; infix[i].type != 0; i++) {
    if (infix[i].type == S21_NUMBER || infix[i].type == S21_VARIABLE) {
      postfix[j++] = infix[i];
    } else if (stk_empty(&stack) || infix[i].value == '(') {
      stk_push(&stack, infix[i]);
    } else if (infix[i].value == ')') {
      j += pullContentOfParents(&stack, &postfix[j]);
    } else if (stk_top_type(&stack) < infix[i].type) {
      stk_push(&stack, infix[i]);
    } else if (stk_top_type(&stack) >= infix[i].type) {
      j += pullTokensWithHigherPriority(infix[i].type, &stack, &postfix[j]);
      stk_push(&stack, infix[i]);
    }
  }
  addTokensFromStackToPostfixLine(&stack, &postfix[j]);
}

int pullTokensWithHigherPriority(int priority, token_stack *stack,
                                 s_tokens *postfix) {
  int counter = 0;
  while (stk_top_type(stack) >= priority) {
    postfix[counter] = stk_pop(stack);
    counter++;
    if (stk_empty(stack)) break;
  }
  return counter;
}

int pullContentOfParents(token_stack *stack, s_tokens *postfix) {
  int counter = 0;
  s_tokens token = stk_pop(stack);
  while (token.value != '(') {
    postfix[counter] = token;
    counter++;
    token = stk_pop(stack);
  }
  return counter;
}

void addTokensFromStackToPostfixLine(token_stack *stack, s_tokens *postfix) {
  for (int j = 0; !stk_empty(stack); j++) {
    postfix[j] = stk_pop(stack);
  }
}

/*

char *infix_to_postfix(char const infix[], char postfix[]) {
  stack s;
  char x;
  int i = 0, j = 0;  // i-index of infix, j-index of postfix
  init(&s);
  for (i = 0; infix[i]; i++) {
    char token;
    token = infix[i];
    if (isalnum_21(token)) {
      while (isalnum_21(infix[i])) {
        postfix[j++] = infix[i];
        i++;
      }
      postfix[j++] = ' ';
      i--;
    } else if (token == '(')
      push(&s, '(');
    else if (token == ')')
      while ((x = pop(&s)) != '(') postfix[j++] = x;
    else {
      while (precedence(token) <= precedence(top(&s)) && !empty(&s)) {
        x = pop(&s);
        postfix[j++] = x;
      }
      push(&s, token);
    }
  }
  while (!empty(&s)) {
    x = pop(&s);
    postfix[j++] = x;
  }
  postfix[j] = '\0';
  return postfix;
}

int precedence(char x) {
  int m = 5;

  if (x == '(') m = 0;

  if (x == '^') m = 3;

  if (x == 'C' || x == 'S' || x == 'T' || x == 'K' || x == 'Q' || x == 'L')
    m = 3;

  if (x == '+' || x == '-') m = 1;

  if (x == '*' || x == '/') m = 2;

  return m;
}

int isalnum_21(char symb) {
  return (symb >= '0' && symb <= '9') || (symb == 'x') || (symb == '.');
}
*/