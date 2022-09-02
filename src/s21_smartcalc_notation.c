#include "s21_smartcalc.h"

// Operator supported: +,-,*,/,%,^,(,)
// Operands supported: all single character operands

char *infix_to_postfix(char const infix[], char postfix[]) {
  stack s;
  char x;
  int i = 0, j;  // i-index of infix,j-index of postfix
  init(&s);
  j = 0;
  for (i = 0; infix[i] != '\0'; i++) {
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
  int m = 0;
  if ((symb >= '0' && symb <= '9') || (symb == 'x') || (symb == '.')) m = 1;
  return m;
}
