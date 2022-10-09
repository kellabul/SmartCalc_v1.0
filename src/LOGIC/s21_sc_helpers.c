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

int isNotNumberInString(char *string) {
  int result = 0;
  int point_count = 0;
  if ((!isNumber(string[0]) && (string[0] != '-' && string[0] != '+')) ||
      !isNumber(string[strlen(string) - 1])) {
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

void convertToFinancialOutputNumber(char *input, char *output) {
  int length = strlen(input);
  int i = length - 1;
  int j = 0;
  for (int h = 0; h < 6 && i > -1; h++) {
    output[j++] = input[i--];
  }
  if (length > 6) {
    for (int h = 0; i > -1; h++) {
      if (h % 3 == 0) {
        output[j++] = ' ';
      }
      output[j++] = input[i--];
    }
  }
  output[j] = '\0';
  reverse_string(output);
}

void reverse_string(char *string) {
  char buffer;
  int end = strlen(string) - 1;
  for (int start = 0; end > start; start++, end--) {
    buffer = string[start];
    string[start] = string[end];
    string[end] = buffer;
  }
}