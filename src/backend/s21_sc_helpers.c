#include <stdio.h>
#include <string.h>

#include "../s21_smartcalc.h"

void convertTokensToString(s_tokens *tokens, char *string) {
  for (int i = 0; tokens[i].type; i++) {
    char buffer_string[100] = {};
    if (tokens[i].type == S21_NUMBER) {
      sprintf(buffer_string, "%Lg", tokens[i].value);
    } else {
      sprintf(buffer_string, "%c", (char)tokens[i].value);
    }
    strcat(string, buffer_string);
  }
}