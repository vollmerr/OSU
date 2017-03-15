#include "headers.h"

void print_error(const char* err, int code) {
  printf("%s", err);
  exit(code);
}

void print_usage(const char* prog, const char* msg) {
  printf("USAGE: %s %s\n", prog, msg);
  exit(EXIT_FAILURE);
}

/**
 * Checks if input is valid
 * @param  msg [description]
 * @return     [description]
 */
void valid_input(const char* msg) {
  char* c = msg;
  // check all chars are valid
  while (*c) {
    if (!strchr(VALID_CHARS, *c)) {
      print_error("ERROR: invalid characters", EXIT_FAILURE);
    }
    c++;
  }
}

/**
 * Converts a character to int representation based off valid chars
 * @param  c [description]
 * @return   [description]
 */
int char_to_i(const char c) {
  int i=0;
  int max = 27;
  while (c != VALID_CHARS[i] && i < max) {
    i++;
  }
  return i;
}

/**
 * Converts an int to char based off valid chars
 * @param  c [description]
 * @return   [description]
 */
char char_from_i(const int c) {
  int i=0;
  int max = 27;
  while (i != VALID_CHARS[c] && i < max) {
    i++;
  }
  return VALID_CHARS[c];
}

// int valid_length(const char* msg, ) {
//   char* c;
//   // check all chars are valid
//   while (*c) {
//     if (!strchr(VALID_CHARS, *c)) {
//       return 0;
//     }
//     c++;
//   }
//   return 1;
// }
