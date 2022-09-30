#include <stdio.h>
#include <string.h>

#include "../s21_smartcalc.h"

void convertTokensToString(s_tokens *tokens, char *string) {
  for (int i = 0; tokens[i].type; i++) {
    char buffer_string[100] = {};
    if (tokens[i].type == S21_NUMBER) {
      sprintf(buffer_string, "%g", tokens[i].value);
    } else {
      sprintf(buffer_string, "%c", (char)tokens[i].value);
    }
    strcat(string, buffer_string);
  }
}

int isNotIntegerInString(char *string) {
  int result = 0;
  for (int i = 0; string[i]; i++) {
    if (string[i] < '0' || string[i] > '9') {
      result = 1;
      break;
    }
  }
  return result;
}

int isNotNumberInString(char *string) {
  int result = 0;
  int point_count = 0;
  if (string[0] < '0' || string[0] > '9' || string[strlen(string) - 1] < '0' ||
      string[strlen(string) - 1] > '9') {
    result = 1;
  } else {
    for (int i = 1; string[i]; i++) {
      if (string[i] == '.') {
        point_count++;
      } else if (string[i] < '0' || string[i] > '9') {
        result = 1;
        break;
      }
    }
  }
  if (point_count > 1) result = 1;
  return result;
}